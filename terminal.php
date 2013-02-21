<?php

define('SERIAL_DEVICE', '/dev/ttyACM0');
/**
 * Mac users will have to use something similar to this:
 * define('SERIAL_DEVICE', '/dev/cu.usbmodem1431');
 *
 */

function readMessage($fp) {
	$header = fread($fp, 2);
	$len = ord($header[0]) + (ord($header[1])<<8);
	if( $len == 0 ) {
		return '';
	}
	return fread($fp, $len);
}

function sendMessage( $fp, $str ) {
	$len = strlen( $str );
	$msg = chr( $len & 0xFF ) . chr( ( $len>>8 ) & 0xFF );
	$msg .= $str;

	fwrite($fp, $msg, $len + 2 );
	
	$ret = readMessage($fp);
	if( substr($ret,0,2) != 'ok' ) {
		return false;
	}
	
	$_len = ord($ret[2]) + (ord($ret[3]) << 8);

	if( $len != $_len ) {
		return false;
	}
	
	return true;
}

$fp = fopen(SERIAL_DEVICE, "w+");
if( !$fp) { 
	die('can\'t open ' . SERIAL_DEVICE);
}

while( true ) {
	$msg = readline('> ');
	if( $msg == 'exit' ) {
		break;
	}
	
	echo "* sending... ";
	if( sendMessage($fp, $msg) ) {
		echo "OK\n\n";
	}
	else {
		echo "FAILED!!!\n\n";
	}
	
	echo readMessage($fp) . "\n";
}

fclose($fp);

