#include "util.h"

namespace util {
	
	void swap32( mess32_t& value ) {	// Byte-Swapping 32 (1 of 2)
		#if LITTLE_ENDIAN
		mess32_t temp = value;
		value.str[0] = temp.str[3];
		value.str[1] = temp.str[2];
		value.str[2] = temp.str[1];
		value.str[3] = temp.str[0];
		#endif
	}
	void swap16( mess16_t& value ) {	// Byte-Swapping 16 (1 of 2)
		#if LITTLE_ENDIAN
		mess16_t temp = value;
		value.str[0] = temp.str[1];
		value.str[1] = temp.str[0];
		#endif
	}

	
	uint32_t getField( uint32_t input, uint32_t start, uint32_t len ) {
		uint32_t output;
		uint32_t mask = 0;										// Initialize mask to 0
		for ( uint32_t i = 0; i < len; i++ ) {					// Generate mask
			mask = ( mask << 1 ) | 1;
		}
		output = ( input >> ( 32 - ( start + len ) ) ) & mask;	// Shift field over then sanitize to get output
		return output;
	}
	void setField( uint32_t& output, uint32_t input, uint32_t start, uint32_t len ) {
		uint32_t mask = 0;										// Initialize mask to 0
		for ( uint32_t i = 1; i <= len; i++ ) {					// Generate mask of correct size
			mask = ( mask << 1 ) | 1;
		}
		mask = mask << ( 32 - len - start );					// Shift mask to cover destination
		output = ( output & ~mask ) | ( ( input << ( 32 - ( start + len ) ) & mask ) );	// Clear destination bits then place sanitized field in output
	}
	uint8_t  fstream_B::get_u08(  ) {		// Read u8
		mess8_t temp;
		read( temp.str, 1 );
		return temp.u8;
	}
	uint16_t fstream_B::get_u16(  ) {		// Read u16
		mess16_t temp;
		read( temp.str, 2 );
		swap16( temp );
		return temp.u16;
	}
	uint32_t fstream_B::get_u32(  ) {		// Read u32
		mess32_t temp;
		read( temp.str, 4 );
		swap32( temp );
		return temp.u32;
	}
	int8_t   fstream_B::get_s08(  ) {		// Read s8
		mess8_t temp;
		read( temp.str, 1 );
		return temp.s8;
	}
	int16_t  fstream_B::get_s16(  ) {		// Read s16
		mess16_t temp;
		read( temp.str, 2 );
		swap16( temp );
		return temp.s16;
	}
	int32_t  fstream_B::get_s32(  ) {		// Read s32
		mess32_t temp;
		read( temp.str, 4 );
		swap32( temp );
		return temp.s32;
	}
	float    fstream_B::get_flt(  ) {		// Read float
		mess32_t temp;
		read( temp.str, 4 );
		swap32( temp );
		return temp.flt;
	}
	void fstream_B::get_u08( uint8_t&  v ) {	// Read u8
		v = get_u08();
	}
	void fstream_B::get_u16( uint16_t& v ) {	// Read u16
		v = get_u16();
	}
	void fstream_B::get_u32( uint32_t& v ) {	// Read u32
		v = get_u32();
	}
	void fstream_B::get_s08( int8_t&   v ) {	// Read s8
		v = get_s08();
	}
	void fstream_B::get_s16( int16_t&  v ) {	// Read s16
		v = get_s16();
	}
	void fstream_B::get_s32( int32_t&  v ) {	// Read s32
		v = get_s32();
	}
	void fstream_B::get_flt( float&    v ) {	// Read float
		v = get_flt();
	}
	void fstream_B::put_u08( uint8_t  v ) {		// Write u8
		mess8_t* temp = (mess8_t*)&v;		// Create mess32_t that encloses v
		write( temp->str, 1 );				// Write to file stream
	}
	void fstream_B::put_u16( uint16_t v ) {		// Write u16
		mess16_t* temp = (mess16_t*)&v;		// Create mess32_t that encloses v
		swap16( *temp );					// Endian Swap
		write( temp->str, 2 );				// Write to file stream
	}
	void fstream_B::put_u32( uint32_t v ) {		// Write u32
		mess32_t* temp = (mess32_t*)&v;		// Create mess32_t that encloses v
		swap32( *temp );					// Endian Swap
		write( temp->str, 4 );				// Write to file stream
	}
	void fstream_B::put_s08( int8_t   v ) {		// Write s8
		mess8_t* temp = (mess8_t*)&v;		// Create mess32_t that encloses v
		write( temp->str, 1 );				// Write to file stream
	}
	void fstream_B::put_s16( int16_t  v ) {		// Write s16
		mess16_t* temp = (mess16_t*)&v;		// Create mess32_t that encloses v
		swap16( *temp );					// Endian Swap
		write( temp->str, 2 );				// Write to file stream
	}
	void fstream_B::put_s32( int32_t  v ) {		// Write s32
		mess32_t* temp = (mess32_t*)&v;		// Create mess32_t that encloses v
		swap32( *temp );					// Endian Swap
		write( temp->str, 4 );				// Write to file stream
	}
	void fstream_B::put_flt( float    v ) {		// Write float
		mess32_t* temp = (mess32_t*)&v;		// Create mess32_t that encloses v
		swap32( *temp );					// Endian Swap
		write( temp->str, 4 );				// Write to file stream
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
