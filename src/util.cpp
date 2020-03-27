#include "util.h"

namespace util {
	
	uint32_t swap32( uint32_t value ) {	// Byte-Swapping 32 (1 of 2)
		return (
			( ( value & 0xff000000) >> 24 ) | 
			( ( value & 0x00ff0000) >>  8 ) | 
			( ( value & 0x0000ff00) <<  8 ) | 
			( ( value & 0x000000ff) << 24 )
		);
	}
	uint16_t swap16( uint16_t value ) {	// Byte-Swapping 16 (1 of 2)
		return (
			( ( value & 0xff00 ) >>  8) |
			( ( value & 0x00ff ) <<  8)
		);
	}
	int32_t swap32( int32_t value ) {	// Byte-Swapping 32 (2 of 2)
		return (
			( ( value & 0xff000000) >> 24 ) | 
			( ( value & 0x00ff0000) >>  8 ) | 
			( ( value & 0x0000ff00) <<  8 ) | 
			( ( value & 0x000000ff) << 24 )
		);
	}
	int16_t swap16( int16_t value ) {	// Byte-Swapping 16 (2 of 2)
		return (
			( ( value & 0xff00 ) >>  8) |
			( ( value & 0x00ff ) <<  8)
		);
	}
	
	uint32_t getField( uint32_t value, uint32_t start, uint32_t len ) {
		uint32_t mask = 0;											// Initialize mask to 0
		for ( uint32_t i = 0; i < len; i++ ) {						// Generate mask
			mask = ( mask << 1 ) + 1;									// Substitute for adding powers of 2
		}
		return ( ( value >> ( 32 - ( start + len ) ) ) & mask );	// Bitwise math
	}
	uint32_t setField( uint32_t value, uint32_t start, uint32_t len ) {
		uint32_t mask = 0;											// Initialize mask to 0
		for ( uint32_t i = 0; i < len; i++ ) {						// Generate mask
			mask = ( mask << 1 ) + 1;									// Substitute for adding powers of 2
		}
		value = value & (~mask); // Clear field
		return ( ( value << ( 32 - ( start + len ) ) ) & mask );	// Bitwise math
	}
	
	void fstream_B::get( uint8_t&  v ) {	// Read u8
		read( reinterpret_cast<char*>( &v ), 1 );
	}
	void fstream_B::get( uint16_t& v ) {	// Read u16
		read( reinterpret_cast<char*>( &v ), 2 );
		v = util::swap16( v );
	}
	void fstream_B::get( uint32_t& v ) {	// Read u32
		read( reinterpret_cast<char*>( &v ), 4 );
		v = util::swap32( v );
	}
	void fstream_B::get( int8_t&   v ) {	// Read s8
		read( reinterpret_cast<char*>( &v ), 1 );
	}
	void fstream_B::get( int16_t&  v ) {	// Read s16
		read( reinterpret_cast<char*>( &v ), 2 );
		v = util::swap16( v );
	}
	void fstream_B::get( int32_t&  v ) {	// Read s32
		read( reinterpret_cast<char*>( &v ), 4 );
		v = util::swap32( v );
	}
	void fstream_B::get( float&    v ) {	// Read float
		uint32_t temp;
		read( reinterpret_cast<char*>( &temp ), 4 );
		temp = util::swap32( temp );
		v = reinterpret_cast<float&>( temp );
	}
	void fstream_B::put( uint8_t  v ) {	// Read u8
		write( reinterpret_cast<char*>( &v ), 1 );
	}
	void fstream_B::put( uint16_t v ) {	// Read u16
		v = util::swap16( v );
		write( reinterpret_cast<char*>( &v ), 2 );
	}
	void fstream_B::put( uint32_t v ) {	// Read u32
		v = util::swap32( v );
		write( reinterpret_cast<char*>( &v ), 4 );
	}
	void fstream_B::put( int8_t   v ) {	// Read s8
		write( reinterpret_cast<char*>( &v ), 1 );
	}
	void fstream_B::put( int16_t  v ) {	// Read s16
		v = util::swap16( v );
		write( reinterpret_cast<char*>( &v ), 2 );
	}
	void fstream_B::put( int32_t  v ) {	// Read s32
		v = util::swap32( v );
		write( reinterpret_cast<char*>( &v ), 4 );
	}
	void fstream_B::put( float    v ) {	// Read float
		uint32_t temp = reinterpret_cast<uint32_t&>( v );
		temp = swap32( temp );
		write( reinterpret_cast<char*>( &temp ), 4 );
	}
	void fstream_B::skip( std::ios::streamoff offset ) {
		seekp( offset, std::ios::cur );
	}
	void fstream_B::backup_g(  ) {
		history.push( tellg() );
	}
	void fstream_B::backup_p(  ) {
		history.push( tellg() );
	}
	void fstream_B::rewind_g(  ) {
		seekg( history.top() );
		history.pop();
	}
	void fstream_B::rewind_p(  ) {
		seekp( history.top() );
		history.pop();
	}
	void fstream_B::forget(  ) {
		history.pop();
	}
	void fstream_B::amnesia(  ) {
		while ( !history.empty() )
			history.pop();
	}

}
