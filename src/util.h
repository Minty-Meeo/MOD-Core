#ifndef util_h
#define util_h

#include <stdint.h>
#include <fstream>
#include <vector>
#include <stack>
#include <limits>

#define LITTLE_ENDIAN 1
#define LOOP_ME( num ) for ( int i = 0; i < num; i++ )
#define CHECK_IF_OPENED( fstream, filename ) if ( !fstream.is_open() ) { printf( "Could not open \"%s\"\n", filename ); return; }

namespace util {
	
	union mess32_t {
		char     str[4];
		uint32_t u32;
		int32_t  s32;
		float    flt;
		mess32_t() {}
		mess32_t( uint32_t v ) : u32(v) {}
		mess32_t( int32_t v ) : s32(v) {}
		mess32_t( float v ) : flt(v) {}
	};
	union mess16_t {
		char     str[2];
		uint16_t u16;
		int16_t  s16;
		mess16_t() {}
		mess16_t( uint16_t v ) : u16(v) {}
		mess16_t( int16_t v ) : s16(v) {}
	};
	union mess8_t {
		char    str[1];
		uint8_t u8;
		int8_t  s8;
		mess8_t() {}
		mess8_t( uint8_t v ) : u8(v) {}
		mess8_t( int8_t v ) : s8(v) {}
	};
	
	void swap32( mess32_t& );
	void swap16( mess16_t& );
	
	uint32_t getField( uint32_t, uint32_t, uint32_t );
	void setField( uint32_t&, uint32_t, uint32_t, uint32_t );
	
	class fstream_B : public std::fstream { using std::fstream::fstream;
	private:
		std::stack<std::ios::streampos> history;
	public:
		using std::fstream::get;		// Overloaded
		using std::fstream::put;		// Overloaded
		using std::fstream::getline;	// Overloaded
		uint8_t  get_u08(  );		// Read u8
		uint16_t get_u16(  );		// Read u16
		uint32_t get_u32(  );		// Read u32
		int8_t   get_s08(  );		// Read s8
		int16_t  get_s16(  );		// Read s16
		int32_t  get_s32(  );		// Read s32
		float    get_flt(  );		// Read float
		void get_u08( uint8_t&  );	// Read u8
		void get_u16( uint16_t& );	// Read u16
		void get_u32( uint32_t& );	// Read u32
		void get_s08( int8_t&   );	// Read s8
		void get_s16( int16_t&  );	// Read s16
		void get_s32( int32_t&  );	// Read s32
		void get_flt( float&    );	// Read float
		void put_u08( uint8_t  );	// Write u8
		void put_u16( uint16_t );	// Write u16
		void put_u32( uint32_t );	// Write u32
		void put_s08( int8_t   );	// Write s8
		void put_s16( int16_t  );	// Write s16
		void put_s32( int32_t  );	// Write s32
		void put_flt( float    );	// Write float
		void skip( std::ios::streamoff );	// ifstream "ignore" but for ofstream
		std::istream& getline( char* s ) { return getline( s, std::numeric_limits<std::streamsize>::max() ); }
		void backup_g(  );	// Backup input streampos
		void backup_p(  );	// Backup output streampos
		void rewind_g(  );	// Rewind input streampos
		void rewind_p(  );	// Rewind output streampos
		void forget(  );	// Elimintate one record
		void amnesia(  );	// Eliminate all records
	};
	
}

#endif
