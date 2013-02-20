#ifndef SerialHelper_h
#define SerialHelper_h

class SerialHelper {

	public:
		SerialHelper(int bufferSize, int headerLen);
		void read();
		void write(char *message, int len);
		char *getMessage();
		int getMessageLength();
		
	private:
		unsigned int getPacketLength();
		char *_buffer;
		int _bufferSize;
		int _headerLen;
		unsigned int _messageLength;
	
};

#endif