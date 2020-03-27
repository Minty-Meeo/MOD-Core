#ifndef util_h
#define util_h

#include <stdint.h>
#include <fstream>
#include <vector>
#include <stack>
#include <limits>

#define LOOP_ME( num ) for ( int i = 0; i < num; i++ )
#define CHECK_IF_OPENED( fstream, filename ) if ( !fstream.is_open() ) { printf( "Could not open \"%s\"\n", filename ); return; }

namespace util {

	uint32_t swap32( uint32_t );
	uint16_t swap16( uint16_t );
	int32_t swap32( int32_t );
	int16_t swap16( int16_t );
	
	uint32_t getField( uint32_t, uint32_t, uint32_t );
	//uint32_t setField( uint32_t, uint32_t, uint32_t );
	
	class fstream_B : public std::fstream { using std::fstream::fstream;
	private:
		std::stack<std::ios::streampos> history;
	public:
		using std::fstream::get;		// Overloaded
		using std::fstream::put;		// Overloaded
		using std::fstream::getline;	// Overloaded
		void get( uint8_t&  );	// Read u8
		void get( uint16_t& );	// Read u16
		void get( uint32_t& );	// Read u32
		void get( int8_t&   );	// Read s8
		void get( int16_t&  );	// Read s16
		void get( int32_t&  );	// Read s32
		void get( float&    );	// Read float
		void put( uint8_t  );	// Write u8
		void put( uint16_t );	// Write u16
		void put( uint32_t );	// Write u32
		void put( int8_t   );	// Write s8
		void put( int16_t  );	// Write s16
		void put( int32_t  );	// Write s32
		void put( float    );	// Write float
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
