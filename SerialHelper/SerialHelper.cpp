
#include "SerialHelper.h"
#include <Arduino.h>

SerialHelper::SerialHelper(int bufferSize, int headerLen) {
	_buffer = (char *) malloc(bufferSize * sizeof(char));
	_headerLen = headerLen;
	_bufferSize = bufferSize;
}

unsigned int SerialHelper::getPacketLength() {
	unsigned int len = 0;
	int chr, i;
	for( i = 0 ; i < 2 ; i++ ) {
		chr = Serial.read();
		if( chr == -1 ) {
			i--;
			continue;
		}
		
		len += ( chr << (8 * i) );
	}
	
	return len;
}

int SerialHelper::getMessageLength() {
	return _messageLength;
}

char *SerialHelper::getMessage() {
	return _buffer + _headerLen;
}

void SerialHelper::write(char *message, int len) {
	char header[2];
	header[0] = len & 0xFF;
	header[1] = (len >> 8) & 0xFF;
	Serial.write( (uint8_t *) &header, 2 );
	Serial.write( (uint8_t *) message, len );
}

void SerialHelper::read() {
	int i=0, input=0;
	char resp[] = { 'o', 'k', 0x0, 0x0 };
	int bufferSize = _bufferSize - 1; // leave space for 0x00
	_messageLength=getPacketLength();
	memset( _buffer, 0, _bufferSize );
	while( i < _messageLength ) {
		input = Serial.read();
		if( input == -1 ) {
			continue;
		}
		
		if( i < bufferSize ) {
			_buffer[ i ] = input;
		}
		
		i++;
	}
	resp[2] = _messageLength & 0xFF;
	resp[3] = (_messageLength >> 8) & 0xFF;

	write( (char *) &resp, 4 );
	
	if( _messageLength > _bufferSize ) {
		_messageLength = _bufferSize;
	}
}
