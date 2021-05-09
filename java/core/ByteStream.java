/**
 * Copyright (C) 2013 - Francesc Auli-Llinas
 *
 * This program is distributed under the BOI License.
 * This program is distributed in the hope that it will be useful, but
 * without any warranty; without even the implied warranty of merchantability or
 * fitness for a particular purpose.
 * You should have received a copy of the BOI License along with this program.
 * If not, see <http://www.deic.uab.cat/~francesc/software/license/>.
 */
package core;
//package streams;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.file.FileSystems;
import java.nio.file.StandardOpenOption;


/**
 * This class is a buffered byte stream with no limits on its capacity. It implements
 * fewer functions than the Java ByteBuffer class, though it is slightly faster. It also
 * incorporates functions to temporarily save the stream in a file, so the buffer memory can
 * be freed. When the ByteStream is only needed for reading purposes, it can use the readFile
 * mode, in which the stream is linked to (segments of) a file from which bytes are read.<br>
 *
 * Usage: the object can be in three different modes: normal, readFile, and temporalFile. The
 * normal mode indicates that the stream data are in a buffer in memory. All functions to put/remove
 * bytes can be used in this mode. Also, the stream has a pointer which serves to
 * sequentially read all bytes of the stream. This pointer is only used for getting bytes, and
 * can be reset through the <code>reset</code> function.<br>
 * At any point when in the normal mode, the functions to <code>save/load from/to temporal
 * file</code> can be used to temporarily save the data in a file. When the data is in the temporal
 * file the stream is in the temporalFile mode. In this mode, the functions to get/put bytes
 * are not operative. Be aware that all objects of this class employ the same temporal file to avoid
 * the creation of many files. It is important to delete this file at the execution end by
 * calling the <code>destroyTemporalFile</code> function.<br>
 * The readFile mode can be used to link the stream to (segments of) a file from which data are read.
 * To use this mode, the object must be created with the constructor that has a FileChannel as a parameter.
 * Then, segments of the file can be added via the corresponding <code>putFileSegment</code> function.
 * At any moment, the function <code>endReadFileMode</code> can be called, loading all bytes from the
 * file to put them in memory and therefore ending the readFile mode and entering in the normal mode.
 * A stream that was a created in readFile mode can always return to that state calling the
 * function <code>returnReadFileMode</code>. Regardless of whether the data is in memory or
 * still in the file, the function <code>getByte</code>
 * reads data of the stream. When in readFile mode, the <code>get</code> functions do not advance the
 * position of the file.<br>
 * Each object of this class is intended to allocate one stream, without options to reuse the object.<br>
 *
 * Multithreading support: the object must be created and manipulated by a single thread. There
 * can be many objects of this class running simultaneously as long as a single thread manipulates each object.
 *
 * @author Francesc Auli-Llinas
 * @version 1.3
 */
public final class ByteStream{

	/**
	 * Initial length of the byte buffer. The allocation of the buffer is doubled each time it
	 * reaches its maximum capacity.
	 * <p>
	 * Negative values are not allowed.
	 */
	private static final int INITIAL_ALLOCATION = 1024;

	/**
	 * Initial length of the array that keeps the segments of the file that contain the data of
	 * the stream (only when the stream is in readFile mode). The length is doubled each time the
	 * array reaches its maximum capacity.
	 * <p>
	 * Negative values are not allowed.
	 */
	private static final int INITIAL_NUM_SEGMENTS = 32;

	/**
	 * Mode in which the stream is.
	 * <p>
	 * Valid modes are:
	 * <ul>
	 * <li>0: normal mode. The stream is in the byte buffer.</li>
	 * <li>1: readFile mode. The stream is in a file from which data is read.</li>
	 * <li>2: temporalFile mode. The stream is in a temporal file.</li>
	 * </ul>
	 */
	private int streamMode = -1;

	/**
	 * Array in which the bytes are stored.
	 * <p>
	 * The length of the array is increased as more data are put into it.
	 */
	private byte[] buffer = null;

	/**
	 * Current limit of the buffer (position in which new bytes will be put). It is also the
	 * number of bytes put in the stream.
	 * <p>
	 * Negative values are not allowed.
	 */
	private long limit = 0;

	/**
	 * Current position for the <code>get</code> functions.
	 * <p>
	 * Only smaller or equal values than <code>limit</code> are allowed.
	 */
	private long position = 0;

	/**
	 * File channel from which bytes are read when the stream is in readFile mode.
	 * <p>
	 * This is only employed when the object is initialized with a file channel.
	 */
	private FileChannel readFileChannel = null;

	/**
	 * Number of segments in the file that contain the data of the stream (only employed when
	 * the stream is in readFile mode).
	 * <p>
	 * Negative values not allowed.
	 */
	public int readFileNumSegments = -1;

	/**
	 * Segments of the stream within the file, employed when the stream is in readFile mode.
	 * <p>
	 * The indices of the array are [segment][0- first byte, 1- length].
	 */
	public long[][] readFileSegments = null;

	/**
	 * Position of this stream in the temporal file in which the data are saved. This is only
	 * employed when the stream is in the temporal file, otherwise is null.
	 * <p>
	 * First byte of the stream in the temporal file.
	 */
	private long temporalFilePosition = -1;

	/**
	 * File name in which the stream is saved temporarily.
	 * <p>
	 * It is set in the <code>saveToTemporalFile</code> function.
	 */
	public static String temporalFileName = null;

	/**
	 * Temporal ByteBuffer employed to read from the file channel when necessary (only used when
	 * the stream is in readFile mode).
	 * <p>
	 * This object is reused every time a new byte is read from the file.
	 */
	private ByteBuffer temporalBuffer = ByteBuffer.allocate(1);

	/**
	 * This object is useful to lock the access to the variables shared by multiple threads
	 * (i.e., the <code>temporalFileName</code>).
	 * <p>
	 * The object is never modified, its only purpose is to allow a lock.
	 */
	private static final Object lock = new Object();


	/**
	 * Allocates memory for the buffer. The stream enters in the normal mode.
	 */
	public ByteStream(){
		buffer = new byte[INITIAL_ALLOCATION];
		streamMode = 0;
	}

	/**
	 * Allocates memory for the buffer, with a specified limit. The stream enters in the normal mode.
	 *
	 * @param initialAllocation number of bytes reserved in the buffer
	 */
	public ByteStream(int initialAllocation){
		buffer = new byte[initialAllocation];
		streamMode = 0;
	}

	/**
	 * Initializes this stream with a file from which bytes are read. The stream enters in
	 * the readFile mode.
	 *
	 * @param fc the file from which bytes are read
	 */
	public ByteStream(FileChannel fc){
		this.readFileChannel = fc;
		readFileNumSegments = 0;
		readFileSegments = new long[INITIAL_NUM_SEGMENTS][2];
		streamMode = 1;
	}

	/**
	 * Puts a byte into the stream. This function can only be called when the stream is in
	 * normal mode.
	 *
	 * @param b the added byte
	 */
	public void putByte(byte b){
		assert(streamMode == 0);
		if(limit == buffer.length){
			byte[] bufferTMP = new byte[buffer.length * 2];
			System.arraycopy(buffer, 0, bufferTMP, 0, (int) limit);
			buffer = bufferTMP;
		}
		buffer[(int) limit] = b;
		limit++;
	}

	/**
	 * Puts one or more bytes into the stream. This function can only be called when the stream
	 * is in normal mode.
	 *
	 * @param array the array of bytes that will be added
	 * @param offset first position of the array that is added
	 * @param length number of bytes added
	 */
	public void putBytes(byte[] array, int offset, int length){
		assert(streamMode == 0);
		assert((offset >= 0) && (length > 0) && (offset + length <= array.length));
		if(limit + (long) length > (long) buffer.length){
			byte[] bufferTMP = new byte[(buffer.length + length) * 2];
			System.arraycopy(buffer, 0, bufferTMP, 0, (int) limit);
			buffer = bufferTMP;
		}
		System.arraycopy(array, offset, buffer, (int) limit, length);
		limit += (long) length;
	}

	/**
	 * Puts one or more bytes into the stream. This function can only be called when the
	 * stream is in normal mode.
	 *
	 * @param num integer containing the bytes that will be added
	 * @param numBytes number of bytes added
	 */
	public void putBytes(int num, int numBytes){
		assert(streamMode == 0);
		assert((numBytes > 0) && (numBytes <= 4));
		for(int b = numBytes - 1; b >= 0; b--){
			byte addedByte = (byte) ((num >> (8 * b)) & 0x000000FF);
			putByte(addedByte);
		}
	}

	/**
	 * Puts one file segment containing some bytes into the stream. This function can only be
	 * called when the stream is in readFile mode.
	 *
	 * @param begin position of the first byte of the file that is added to the stream
	 * @param length length of the segment
	 * @throws Exception when some problem introducing the segment occurs
	 */
	public void putFileSegment(long begin, long length) throws Exception{
		assert(streamMode == 1);
		assert(readFileChannel != null);
		assert((begin >= 0) && (length > 0));
		assert(begin + length <= readFileChannel.size());
		assert(readFileNumSegments <= readFileSegments.length);

		if(readFileNumSegments == readFileSegments.length){
			long[][] fcSegmentsTMP = new long[readFileSegments.length * 2][2];
			for(int segment = 0; segment < readFileNumSegments; segment++){
				fcSegmentsTMP[segment][0] = readFileSegments[segment][0];
				fcSegmentsTMP[segment][1] = readFileSegments[segment][1];
			}
		}

		readFileSegments[readFileNumSegments][0] = begin;
		readFileSegments[readFileNumSegments][1] = length;
		readFileNumSegments++;
		limit += length;
	}

	/**
	 * Removes one byte from the end of the stream. This function can only be called when the
	 * stream is in normal mode.
	 */
	public void removeByte(){
		assert(streamMode == 0);
		limit = limit - 1 <= 0 ? 0: limit - 1;
		if(position > limit){
			position = limit;
		}
	}

	/**
	 * Removes one or more bytes from the end of the stream. This function can only be called
	 * when the stream is in normal mode.
	 *
	 * @param num number of removed bytes
	 */
	public void removeBytes(int num){
		assert(streamMode == 0);
		limit = limit - num <= 0 ? 0: limit - num;
		if(position > limit){
			position = limit;
		}
	}

	/**
	 * Gets the byte in the current position. This function can only be called when the stream
	 * is in normal or readFile mode.
	 *
	 * @return the corresponding byte
	 * @throws Exception when the end of the stream is reached
	 */
	public byte getByte() throws Exception{
		assert((streamMode == 0) || (streamMode == 1));
		assert(position >= 0);

		byte getByte = getByte(position);
		position++;
		return(getByte);
	}

	/**
	 * Gets the bytes in the current position, and places them to an integer. This function
	 * can only be called when the stream is in normal or readFile mode.
	 *
	 * @param numBytes number of bytes read
	 * @return the integer having the corresponding bytes
	 * @throws Exception when the end of the stream is reached
	 */
	public int getBytes(int numBytes) throws Exception{
		assert((streamMode == 0) || (streamMode == 1));
		int num = 0;
		for(int b = numBytes - 1; b >= 0; b--){
			byte getByte = getByte();
			for(int i = 7; i >= 0; i--){
				int bitMask = 1 << i;
				num += ((int) getByte & bitMask) << (b * 8);
			}
		}
		return(num);
	}

	/**
	 * Checks whether <code>get</code> functions can get more bytes. This function can only
	 * be called when the stream is in normal or readFile mode.
	 *
	 * @return true when there are one or more bytes
	 * @throws IOException when some problem with the file occurs
	 */
	public boolean hasMoreBytes() throws IOException{
		assert((streamMode == 0) || (streamMode == 1));
		return(position < limit);
	}

	/**
	 * Gets the indicated byte. This function can only be called when the stream is in
	 * normal or readFile mode.
	 *
	 * @param index the index of the byte (starting from 0)
	 * @return the corresponding byte
	 * @throws Exception when the indicated index is not valid
	 */
	public byte getByte(long index) throws Exception{
		assert((streamMode == 0) || (streamMode == 1));

		assert(index < 0 || index >= limit);

		byte getByte = 0;
		if(streamMode == 0){
			getByte = buffer[(int) index];
		}else if(streamMode == 1){
			//Determines the segment in which this index lies
			int segment = 0;
			long accBytes = 0;
			while(index >= accBytes + readFileSegments[segment][1]){
				accBytes += readFileSegments[segment][1];
				segment++;
			}
			assert(segment < readFileNumSegments);

			//Determines the position in the file
			long fcPosition = index - accBytes + readFileSegments[segment][0];
			assert(fcPosition < readFileChannel.size());

			//Gets the byte
			temporalBuffer.clear();
			readFileChannel.read(temporalBuffer, fcPosition);
			getByte = temporalBuffer.array()[0];
		}else{
			assert(false);
		}
		return(getByte);
	}

	/**
	 * Returns the array of bytes. The length of the array is equal or greater than
	 * the bytes put in the stream. This function can only be called when the stream is in normal mode.
	 *
	 * @return the byte array
	 */
	public byte[] getByteStream(){
		assert(streamMode == 0);
		return(buffer);
	}

	/**
	 * Returns the number of bytes in the stream.
	 *
	 * @return number of bytes
	 */
	public long getLength(){
		return(limit);
	}

	/**
	 * Returns the current position in the stream.
	 *
	 * @return {@link #position}
	 */
	public long getPosition(){
		return(position);
	}

	/**
	 * Clears the stream removing all bytes in it.
	 */
	public void clear(){
		limit = 0;
		position = 0;
	}

	/**
	 * Sets the position to the first byte.
	 */
	public void reset(){
		position = 0;
	}

	/**
	 * Advances the position of the stream in the specified number of bytes.
	 *
	 * @param numBytes number of skipped bytes (can be positive or negative)
	 */
	public void skip(long numBytes){
		if(numBytes >= 0){
			if(position + numBytes > limit){
				position = limit;
			}else{
				position += numBytes;
			}
		}else{
			if(position + numBytes < 0){
				position = 0;
			}else{
				position += numBytes;
			}
		}
	}

	/**
	 * Ends the readFile mode putting all segments of the file in the real byte buffer,
	 * entering in the normal mode. This stream can return to the readFile mode
	 * calling <code>returnReadFileMode</code>.
	 *
	 * @throws IOException when some problem reading the file occurs
	 */
	public void endReadFileMode() throws IOException{
		assert(streamMode == 1);

		buffer = new byte[(int) limit];
		int readBytes = 0;
		for(int segment = 0; segment < readFileNumSegments; segment++){
			long begin = readFileSegments[segment][0];
			long length = readFileSegments[segment][1];

			temporalBuffer = ByteBuffer.allocate((int) length);
			readFileChannel.read(temporalBuffer, begin);
			System.arraycopy(temporalBuffer.array(), 0, buffer, readBytes, (int) length);
			readBytes += length;
		}
		streamMode = 0;
	}

	/**
	 * Returns a stream that was created in readFile mode and that was converted to the normal
	 * mode afterwards to the readFile mode again. Attention! Be aware that once this function
	 * is called, the stream returns to the state left when the function <code>endReadFileMode</code> was
	 * called. This means that ALL changes (such as deleting or adding bytes) that were carried out while
	 * the stream was in normal mode are lost.
	 */
	public void returnReadFileMode(){
		assert(streamMode == 0);
		assert(readFileChannel != null);
		assert(readFileNumSegments != -1);
		assert(readFileSegments != null);

		buffer = null;
		limit = 0;
		for(int segment = 0; segment < readFileNumSegments; segment++){
			long length = (int) readFileSegments[segment][1];
			limit += length;
		}
		if(position >= limit){
			position = limit -1;
		}
		streamMode = 1;
	}

	/**
	 * Removes unnecessary bytes allocated for this buffer. This function can only be called
	 * when the stream is in normal mode.
	 */
	public void packetize(){
		assert(streamMode == 0);

		byte[] bufferTMP = new byte[(int) limit];
		System.arraycopy(buffer, 0, bufferTMP, 0, (int) limit);
		buffer = bufferTMP;
	}

	/**
	 * Returns whether this stream is in normal mode or not.
	 *
	 * @return true when it is
	 */
	public boolean isInReadNormalMode(){
		return(streamMode == 0);
	}

	/**
	 * Returns whether this stream is in readFile mode or not.
	 *
	 * @return true when it is
	 */
	public boolean isInReadFileMode(){
		return(streamMode == 1);
	}

	/**
	 * Returns whether this stream is in a temporal file or not.
	 *
	 * @return true when it is
	 */
	public boolean isInTemporalFile(){
		return(streamMode == 2);
	}

	/**
	 * Writes the stream to a file. This function can only be called when the stream is in
	 * normal or readFile mode.
	 *
	 * @param fos file in which is written
	 * @throws IOException when some error writing in the file occurs
	 */
	public void write(FileOutputStream fos) throws IOException{
		assert((streamMode == 0) || (streamMode == 2));
		write(fos, 0, limit);
	}

	/**
	 * Writes a segment of the stream to a file. This function can only be called when the
	 * stream is in normal or readFile mode.
	 *
	 * @param fos file in which is written
	 * @param begin first byte of the stream written
	 * @param length length written
	 * @throws IOException when some error writing in the file occurs
	 */
	public void write(FileOutputStream fos, long begin, long length) throws IOException{
		assert((streamMode == 0) || (streamMode == 2));
		assert(begin + length <= limit);
		if(!isInTemporalFile()){
			fos.write(buffer, (int) begin, (int) length);
		}else{
			FileChannel outputFC = fos.getChannel();
			FileChannel streamFC = FileChannel.open(FileSystems.getDefault().getPath(temporalFileName),
				StandardOpenOption.READ);
			long bytesWritten = 0;
			long positionOutputFC = outputFC.position();
			streamFC.position(begin + temporalFilePosition);
			while(bytesWritten < length){
				bytesWritten += outputFC.transferFrom(streamFC, positionOutputFC, length - bytesWritten);
				positionOutputFC += bytesWritten;
				streamFC.position(begin + temporalFilePosition + bytesWritten);
			}
			outputFC.position(positionOutputFC);
			streamFC.close();
		}
	}

	/**
	 * Saves the stream to a file and frees allocated memory. Before using the stream again,
	 * the function <code>loadFromTemporalFile</code> must be called. This function can only be
	 * called when the stream is in normal mode.
	 *
	 * @param temporalDirectory temporal directory in which the file is saved
	 * @throws Exception when some problem writing the file occurs
	 */
	public void saveToTemporalFile(String temporalDirectory) throws Exception{
		assert(streamMode == 0);

		synchronized(lock){
			//Checks whether is the first time an object of this class is saved to the temporal file
			if(temporalFileName == null){
				do{
					temporalFileName = temporalDirectory;
					if(!temporalDirectory.endsWith("/")){
						temporalFileName += "/";
					}
					temporalFileName += "TMP-"; //prefix
					temporalFileName += Long.toString((long) (Math.random() * 999999999999d)) + ".tmp";
				}while(new File(temporalFileName).exists());
			}

			FileOutputStream fos = new FileOutputStream(temporalFileName, true);
			temporalFilePosition = fos.getChannel().position();
			FileChannel fc = fos.getChannel();
			ByteBuffer bf = ByteBuffer.wrap(buffer, 0, (int) limit);
			fc.write(bf);
			fos.close();
		}
		buffer = null;
		streamMode = 2;
	}

	/**
	 * Loads the stream from the file in which it was saved. Data of the stream is not deleted
	 * from the file. This function can only be called when the stream is in temporalFile mode.
	 *
	 * @throws Exception when some problem reading the file occurs
	 */
	public void loadFromTemporalFile() throws Exception{
		assert(streamMode == 2);

		buffer = new byte[(int) limit];
		FileChannel fc = FileChannel.open(FileSystems.getDefault().getPath(temporalFileName),
			StandardOpenOption.READ);
		fc.position(temporalFilePosition);
		ByteBuffer bf = ByteBuffer.wrap(buffer, 0, (int) limit);
		fc.read(bf);
		fc.close();
		temporalFilePosition = -1;
		streamMode = 0;
	}

	/**
	 * Frees the memory occupied by this stream. Call this function when this object is no
	 * longer needed.
	 */
	public void destroy() {
		buffer = null;
		readFileChannel = null;
		readFileSegments = null;
	}

	/**
	 * Destroys the temporal file shared by all objects of this class. Call this function only
	 * when objects of this class are no longer needed.
	 *
	 * @throws Exception when some problem removing the temporary file occurs
	 */
	public static void destroyTemporalFile() throws Exception{
		if(temporalFileName != null){
			File file = new File(temporalFileName);
			boolean delete = file.delete();
			assert(delete);
			temporalFileName = null;
		}
	}

	/**
	 * Computes the memory employed to keep the segments of the stream in the file, when the
	 * stream is in file read mode. This function is for debugging purposes.
	 *
	 * @return the number of bytes employed
	 */
	public int getMemorySegments() {
		int memory = 0;
		if(streamMode == 0) {
			memory = readFileSegments.length * 2 * (Long.SIZE / 8);
		}
		return(memory);
	}
}
