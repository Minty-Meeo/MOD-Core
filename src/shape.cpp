#include "shape.h"

namespace model3D {
	void xyz::normalize() {
		float magnitude = sqrt( ( x * x ) + ( y * y ) + ( z * z ) );
		x = x / magnitude;
		y = y / magnitude;
		z = z / magnitude;
	}
	void colTri::recalibrate( std::vector<xyz>& vtxPos ) {
		xyz& A = vtxPos[index_a];							// Lookup Vertex A
		xyz& B = vtxPos[index_b];							// Lookup Vertex B
		xyz& C = vtxPos[index_c];							// Lookup Vertex C
		xyz V ( (B.x - A.x), (B.y - A.y), (B.z - A.z) );	// V is vector AB, shifted to start at origin
		xyz W ( (C.x - A.x), (C.y - A.y), (C.z - A.z) );	// W is vector AC, shifted to start at origin
		tri_normal.x = ( V.y * W.z ) - ( V.z * W.y );		// Cross Product X (solving surface normal)
		tri_normal.y = ( V.z * W.x ) - ( V.x * W.y );		// Cross Product Y (solving surface normal)
		tri_normal.z = ( V.x * W.y ) - ( V.y * W.x );		// Cross Product Z (solving surface normal)
		tri_normal.normalize();								// Normalize       (solving surface normal)
		xyz middle (										// Middle is the average of A, B, and C.
			( A.x + B.x + C.x ) / 3,	// X
			( A.y + B.y + C.y ) / 3,	// Y
			( A.z + B.z + C.z ) / 3		// Z
		);
		magnitude = (										// Magnitude is a value used in collision math
			( tri_normal.x * middle.x ) +
			( tri_normal.y * middle.y ) +
			( tri_normal.z * middle.z )
		);
	}
	
	/// i_Shape private subfunctions
	void shape::i_Shape_0000( util::fstream_B& inFile ) {	// info
		uint32_t bitfield_temp;
		inFile.ignore( 0x18 );	// Skip padding
		inFile.get_u16( info.year );
		inFile.get_u08( info.month );
		inFile.get_u08( info.date );
		inFile.get_u32( bitfield_temp );
		info.scalingrule = util::getField( bitfield_temp, 31, 1 );
//		info.???         = util::getField( bitfield_temp, 30, 1 );
//		info.???         = util::getField( bitfield_temp, 29, 1 );
		info.embossbump  = util::getField( bitfield_temp, 28, 1 );
//		info.???         = util::getField( bitfield_temp, 21, 1 );
	}
	void shape::i_Shape_0010( util::fstream_B& inFile ) {	// vtxPos
		uint32_t packetCount; inFile.get_u32( packetCount );		// Read packet count
		
		inFile.ignore( 0x14 );									// Skip Padding
		xyz temp;
		for ( ; packetCount > 0; packetCount-- ) {
			inFile.get_flt( temp.x );
			inFile.get_flt( temp.y );
			inFile.get_flt( temp.z );
			vtxPos.push_back( temp );
		}
	}
	void shape::i_Shape_0013( util::fstream_B& inFile ) {	// vtxColor
		uint32_t packetCount; inFile.get_u32( packetCount );		// Read packet count
		
		inFile.ignore( 0x14 );									// Skip Padding
		color temp;
		for ( ; packetCount > 0; packetCount-- ) {
			inFile.get_u32( temp.rgba );
			vtxColor.push_back( temp );
		}
	}
	void shape::i_Shape_0011( util::fstream_B& inFile ) {	// vtxNrm
		uint32_t packetCount; inFile.get_u32( packetCount );		// Read packet count
		inFile.ignore( 0x14 );									// Skip Padding
		xyz temp;
		for ( ; packetCount > 0; packetCount-- ) {
			inFile.get_flt( temp.x );
			inFile.get_flt( temp.y );
			inFile.get_flt( temp.z );
			vtxNrm.push_back( temp );
		}
	}
	void shape::i_Shape_0012( util::fstream_B& inFile ) {	// vtxNBT
		uint32_t packetCount; inFile.get_u32( packetCount );		// Read packet count
		inFile.ignore( 0x14 );									// Skip Padding
		nbt temp;
		for ( ; packetCount > 0; packetCount-- ) {
			inFile.get_flt( temp.normal.x );
			inFile.get_flt( temp.normal.y );
			inFile.get_flt( temp.normal.z );
			inFile.get_flt( temp.binormal.x );
			inFile.get_flt( temp.binormal.y );
			inFile.get_flt( temp.binormal.z );
			inFile.get_flt( temp.tangent.x );
			inFile.get_flt( temp.tangent.y );
			inFile.get_flt( temp.tangent.z );
			vtxNBT.push_back( temp );
		}
	}
	void shape::i_Shape_001X( util::fstream_B& inFile, uint32_t i ) {	// texCoord (0 to 7)
		uint32_t packetCount; inFile.get_u32( packetCount );		// Read packet count
		inFile.ignore( 0x14 );									// Skip Padding
		uv temp;
		for ( ; packetCount > 0; packetCount-- ) {
			inFile.get_flt( temp.u );
			inFile.get_flt( temp.v );
			texCoord[i].push_back( temp );
		}
	}
	void shape::i_Shape_0020( util::fstream_B& inFile ) {	// texImg
		uint32_t packet_count; inFile.get_u32( packet_count );		// Read packet count
		inFile.ignore( 0x14 );									// Skip Padding
		mTXE temp;
		std::streamsize data_size;
		for ( ; packet_count > 0; packet_count-- ) {
			inFile.get_u16( temp.res_x );
			inFile.get_u16( temp.res_y );
			inFile.get_u32( temp.format );
			inFile.ignore( 0x04 );	// unknown field
			inFile.ignore( 0x10 );
			inFile.get_s32( data_size );
			
			printf( "%u\n", data_size );
			
//			char data_temp[data_size];
//			inFile.read( data_temp, data_size );
//			temp.data.assign( data_temp );
			temp.data.resize( data_size );
			inFile.read( temp.data.data(), data_size );	// Access vector's char* directly.
			texImg.push_back( temp );
		}
	}
	void shape::i_Shape_0022( util::fstream_B& inFile ) {	// texAttr
		uint32_t packetCount; inFile.get_u32( packetCount );		// Read packet count
		inFile.ignore( 0x14 );									// Skip Padding
		attr temp;
		for ( ; packetCount > 0; packetCount-- ) {
			inFile.get_u16( temp.texture_index );
			inFile.get_u16( temp.pallette_index );
			inFile.get_u08( temp.wrap_s );
			inFile.get_u08( temp.wrap_t );
			inFile.ignore( 6 );	// unknown fields
			texAttr.push_back( temp );
		}
	}
	void shape::i_Shape_0030( util::fstream_B& inFile ) {	// material
		
	}
	void shape::i_Shape_0041( util::fstream_B& inFile ) {	// mtxEvl
		
	}
	void shape::i_Shape_0040( util::fstream_B& inFile ) {	// mtx
		
	}
	void shape::i_Shape_0050( util::fstream_B& inFile ) {	// mesh
		
	}
	void shape::i_Shape_0060( util::fstream_B& inFile ) {	// joint
		
	}
	void shape::i_Shape_0061( util::fstream_B& inFile ) {	// jointName
		
	}
	void shape::i_Shape_0080( util::fstream_B& inFile ) {	// ???
		
	}
	void shape::i_Shape_0100( util::fstream_B& inFile ) {	// colMesh
		uint32_t packetCount; inFile.get_u32( packetCount );		// Read packet count
		/* ??? */ inFile.ignore( 0x04 );
		inFile.ignore( 0x10 );									// Skip Padding
		inFile.ignore( 0x20 );									// Skip Padding
		colTri temp;
		uint32_t mapcode_temp;
		for ( ; packetCount > 0; packetCount-- ) {
			inFile.get_u32( mapcode_temp );
			inFile.get_u32( temp.index_c );
			inFile.get_u32( temp.index_b );
			inFile.get_u32( temp.index_a );
			inFile.ignore( 2 );
			inFile.get_s16( temp.neighbor_ca );
			inFile.get_s16( temp.neighbor_bc );
			inFile.get_s16( temp.neighbor_ab );
			inFile.get_flt( temp.tri_normal.x );
			inFile.get_flt( temp.tri_normal.y );
			inFile.get_flt( temp.tri_normal.z );
			inFile.get_flt( temp.magnitude );
			temp.mapcode.attribute = util::getField( mapcode_temp, 0, 3 );
			temp.mapcode.slip_code = util::getField( mapcode_temp, 3, 2 );
			temp.mapcode.is_bald   = util::getField( mapcode_temp, 5, 1 );
			colMesh.push_back( temp );
		}
	}
	void shape::i_Shape_0110( util::fstream_B& inFile ) {	// ???
		
	}
	void shape::i_Shape_INI ( util::fstream_B& inFile ) {	// INI
		std::ios::streampos ini_start, ini_end;
		std::streamsize ini_size;
		inFile.backup_g();					// Backup filepos
		ini_start = inFile.tellg();			// ini_start
		inFile.seekg( 0, std::ios::end );	// Seek to EOF
		ini_end = inFile.tellg();			// ini_end
		inFile.rewind_g();					// Rewind filepos
		
		ini_size = ini_end - ini_start;		// ini_size
		char temp[ini_size+1] = {0};		// Create temp char*
		inFile.read( temp, ini_size );		// Read INI in entirety
		ini.assign( temp );					// Copy temp to internalized data
	}

	/// o_Shape private subfunctions
	void shape::o_Shape_0000( util::fstream_B& outFile ) {	// info
		uint32_t chunk_size = 0x38;
		outFile.put_u32( 0x0000 );
		outFile.put_u32( chunk_size );
		outFile.backup_p();
		outFile.skip( 0x18 );
		uint32_t bitfield_temp = 0;
		outFile.put_u16( info.year );
		outFile.put_u08( info.month );
		outFile.put_u08( info.date );
		util::setField( bitfield_temp, info.scalingrule, 31, 1 );
//		util::setField( bitfield_temp, info.???        , 30, 1 );
//		util::setField( bitfield_temp, info.???        , 29, 1 );
		util::setField( bitfield_temp, info.embossbump , 28, 1 );
//		util::setField( bitfield_temp, info.???        , 21, 1 );
		outFile.put_u32( bitfield_temp );
		outFile.rewind_p();
		outFile.skip( chunk_size );
	}
	void shape::o_Shape_0010( util::fstream_B& outFile ) {	// vtxPos
		uint32_t chunk_size = calculate_chunk_size( vtxPos.size(), 0x0C );
		outFile.put_u32( 0x0010 );
		outFile.put_u32( chunk_size );
		outFile.backup_p();
		outFile.put_u32( vtxPos.size() );
		outFile.skip( 0x14 );
		for ( uint32_t packetCount = 0; packetCount < vtxPos.size(); packetCount++ ) {
			outFile.put_flt( vtxPos[packetCount].x );
			outFile.put_flt( vtxPos[packetCount].y );
			outFile.put_flt( vtxPos[packetCount].z );
		}
		outFile.rewind_p();
		outFile.skip( chunk_size );
	}
	void shape::o_Shape_0013( util::fstream_B& outFile ) {	// vtxColor
		uint32_t chunk_size = calculate_chunk_size( vtxColor.size(), 0x04 );
		outFile.put_u32( 0x0013 );
		outFile.put_u32( chunk_size );
		outFile.backup_p();
		outFile.put_u32( vtxColor.size() );
		outFile.skip( 0x14 );
		for ( uint32_t packetCount = 0; packetCount < vtxColor.size(); packetCount++ ) {
			outFile.put_u32( vtxColor[packetCount].rgba );
		}
		outFile.rewind_p();
		outFile.skip( chunk_size );
	}
	void shape::o_Shape_0011( util::fstream_B& outFile ) {	// vtxNrm
		uint32_t chunk_size = calculate_chunk_size( vtxNrm.size(), 0x0C );
		outFile.put_u32( 0x0011 );
		outFile.put_u32( chunk_size );
		outFile.backup_p();
		outFile.put_u32( vtxNrm.size() );
		outFile.skip( 0x14 );
		for ( uint32_t packetCount = 0; packetCount < vtxNrm.size(); packetCount++ ) {
			outFile.put_flt( vtxNrm[packetCount].x );
			outFile.put_flt( vtxNrm[packetCount].y );
			outFile.put_flt( vtxNrm[packetCount].z );
		}
		outFile.rewind_p();
		outFile.skip( chunk_size );
	}
	void shape::o_Shape_0012( util::fstream_B& outFile ) {	// vtxNBT
		uint32_t chunk_size = calculate_chunk_size( vtxNBT.size(), 0x24 );
		outFile.put_u32( 0x0012 );
		outFile.put_u32( chunk_size );
		outFile.backup_p();
		outFile.put_u32( vtxNBT.size() );
		outFile.skip( 0x14 );
		for ( uint32_t packetCount = 0; packetCount < vtxNBT.size(); packetCount++ ) {
			outFile.put_flt( vtxNBT[packetCount].normal.x );
			outFile.put_flt( vtxNBT[packetCount].normal.y );
			outFile.put_flt( vtxNBT[packetCount].normal.z );
			outFile.put_flt( vtxNBT[packetCount].binormal.x );
			outFile.put_flt( vtxNBT[packetCount].binormal.y );
			outFile.put_flt( vtxNBT[packetCount].binormal.z );
			outFile.put_flt( vtxNBT[packetCount].tangent.x );
			outFile.put_flt( vtxNBT[packetCount].tangent.y );
			outFile.put_flt( vtxNBT[packetCount].tangent.z );
		}
		outFile.rewind_p();
		outFile.skip( chunk_size );
	}
	void shape::o_Shape_001X( util::fstream_B& outFile, uint32_t i ) {	// texCoord (0 to 7)
		uint32_t chunk_size = calculate_chunk_size( texCoord[i].size(), 0x08 );
		outFile.put_u32( 0x0018 + i );
		outFile.put_u32( chunk_size );
		outFile.backup_p();
		outFile.put_u32( texCoord[i].size() );
		outFile.skip( 0x14 );
		for ( uint32_t packetCount = 0; packetCount < texCoord[i].size(); packetCount++ ) {
			outFile.put_flt( texCoord[i][packetCount].u );
			outFile.put_flt( texCoord[i][packetCount].v );
		}
		outFile.rewind_p();
		outFile.skip( chunk_size );
	}
	void shape::o_Shape_0020( util::fstream_B& outFile ) {	// texImg
		uint32_t chunk_size = 0x18;										// Start with just chunk header size
		for ( uint32_t i = 0; i < texImg.size(); i++ ) 
			chunk_size += 0x20 + texImg[i].data.size() 					// Add size of (mTXE Header + mTXE Data)
			+ ( ( 0x20 - ( texImg[i].data.size() % 0x20 ) ) % 0x20 );	// Resolve 0x20 alignment (in case that's needed?)
		printf( "%u\n", chunk_size );
		outFile.put_u32( 0x0020 );
		outFile.put_u32( chunk_size );
		outFile.backup_p();
		outFile.put_u32( texImg.size() );
		outFile.skip( 0x14 );
		for ( uint32_t packetCount = 0; packetCount < texImg.size(); packetCount++ ) {
			outFile.put_u16( texImg[packetCount].res_x );
			outFile.put_u16( texImg[packetCount].res_y );
			outFile.put_u32( texImg[packetCount].format );
			outFile.skip( 0x04 );	// unknown field
			outFile.skip( 0x10 );
			outFile.put_u32( texImg[packetCount].data.size() );
			outFile.write( texImg[packetCount].data.data(), texImg[packetCount].data.size() );
		}
		outFile.rewind_p();
		outFile.skip( chunk_size );
	}
	void shape::o_Shape_0022( util::fstream_B& outFile ) {	// texAttr
		uint32_t chunk_size = calculate_chunk_size( texAttr.size(), 0x0C );
		outFile.put_u32( 0x0022 );
		outFile.put_u32( chunk_size );
		outFile.backup_p();
		outFile.put_u32( texAttr.size() );
		outFile.skip( 0x14 );
		for ( uint32_t packet_count = 0; packet_count < texAttr.size(); packet_count++ ) {
			outFile.put_u16( texAttr[packet_count].texture_index );
			outFile.put_u16( texAttr[packet_count].pallette_index );
			outFile.put_u08( texAttr[packet_count].wrap_s );
			outFile.put_u08( texAttr[packet_count].wrap_t );
			outFile.skip( 6 );	// unknown fields
		}
		outFile.rewind_p();
		outFile.skip( chunk_size );
	}
	void shape::o_Shape_0030( util::fstream_B& outFile ) {	// material
		
	}
	void shape::o_Shape_0041( util::fstream_B& outFile ) {	// mtxEvl
		
	}
	void shape::o_Shape_0040( util::fstream_B& outFile ) {	// mtx
		
	}
	void shape::o_Shape_0050( util::fstream_B& outFile ) {	// mesh
		
	}
	void shape::o_Shape_0060( util::fstream_B& outFile ) {	// joint
		
	}
	void shape::o_Shape_0061( util::fstream_B& outFile ) {	// jointName
		
	}
	void shape::o_Shape_0080( util::fstream_B& outFile ) {	// ???
		
	}
	void shape::o_Shape_0100( util::fstream_B& outFile ) {	// colMesh
		const uint32_t packet_size = 0x28;
		uint32_t chunk_size = calculate_chunk_size( colMesh.size(), packet_size ) + 0x20;
		outFile.put_u32( 0x0100 );
		outFile.put_u32( chunk_size );
		outFile.backup_p();
		outFile.put_u32( colMesh.size() );
		outFile.skip( 0x14 );
		outFile.skip( 0x20 );
		for ( uint32_t packetCount = 0; packetCount < colMesh.size(); packetCount++ ) {
			outFile.put_u32( colMesh[packetCount].mapcode.u32() );
			outFile.put_u32( colMesh[packetCount].index_c );
			outFile.put_u32( colMesh[packetCount].index_b );
			outFile.put_u32( colMesh[packetCount].index_a );
			outFile.skip( 2 );
			outFile.put_u16( colMesh[packetCount].neighbor_ca );
			outFile.put_u16( colMesh[packetCount].neighbor_bc );
			outFile.put_u16( colMesh[packetCount].neighbor_ab );
			outFile.put_flt( colMesh[packetCount].tri_normal.x );
			outFile.put_flt( colMesh[packetCount].tri_normal.y );
			outFile.put_flt( colMesh[packetCount].tri_normal.z );
			outFile.put_flt( colMesh[packetCount].magnitude );
		}
		outFile.rewind_p();
		outFile.skip( chunk_size );
	}
	void shape::o_Shape_0110( util::fstream_B& outFile ) {	// ???
		
	}
	void shape::o_Shape_FFFF( util::fstream_B& outFile ) {	// EOF
		uint32_t chunk_size = 0x18;
		outFile.put_u32( 0xFFFF );
		outFile.put_u32( chunk_size );
		char temp[ chunk_size ] = {0};
		outFile.write( temp, chunk_size );
	}
	void shape::o_Shape_INI ( util::fstream_B& outFile ) {	// INI
		outFile.write( ini.c_str(), ini.size() );
	}

	/// in, out, manip, etc public functions
	void shape::i_Shape               ( const char* filename ) {
		util::fstream_B inFile ( filename, std::ios::in | std::ios::binary );
		CHECK_IF_OPENED( inFile, filename )
		
		uint32_t chunk_opcode, chunk_size;						// Allocate vars
		while ( 1 ) {
			inFile.get_u32( chunk_opcode );							// Read chunk_opcode
			inFile.get_u32( chunk_size );							// Read chunk_size
			inFile.backup_g();									// Backup to chunk header
			switch ( chunk_opcode ) {
				case 0x0000: i_Shape_0000( inFile );    has_0000 = true; break;
				case 0x0010: i_Shape_0010( inFile );    has_0010 = true; break;
				case 0x0013: i_Shape_0013( inFile );    has_0013 = true; break;
				case 0x0011: i_Shape_0011( inFile );    has_0011 = true; break;
				case 0x0012: i_Shape_0012( inFile );    has_0012 = true; break;
				case 0x0018: i_Shape_001X( inFile, 0 ); has_0018 = true; break;
				case 0x0019: i_Shape_001X( inFile, 1 ); has_0019 = true; break;
				case 0x001A: i_Shape_001X( inFile, 2 ); has_001A = true; break;
				case 0x001B: i_Shape_001X( inFile, 3 ); has_001B = true; break;
				case 0x001C: i_Shape_001X( inFile, 4 ); has_001C = true; break;
				case 0x001D: i_Shape_001X( inFile, 5 ); has_001D = true; break;
				case 0x001E: i_Shape_001X( inFile, 6 ); has_001E = true; break;
				case 0x001F: i_Shape_001X( inFile, 7 ); has_001F = true; break;
				case 0x0020: i_Shape_0020( inFile );    has_0020 = true; break;
				case 0x0022: i_Shape_0022( inFile );    has_0022 = true; break;
				case 0x0030: i_Shape_0030( inFile );    has_0030 = true; break;
				case 0x0041: i_Shape_0041( inFile );    has_0041 = true; break;
				case 0x0040: i_Shape_0040( inFile );    has_0040 = true; break;
				case 0x0050: i_Shape_0050( inFile );    has_0050 = true; break;
				case 0x0060: i_Shape_0060( inFile );    has_0060 = true; break;
				case 0x0061: i_Shape_0061( inFile );    has_0061 = true; break;
				case 0x0080: i_Shape_0080( inFile );    has_0080 = true; break;
				case 0x0100: i_Shape_0100( inFile );    has_0100 = true; break;
				case 0x0110: i_Shape_0110( inFile );    has_0110 = true; break;
				case 0xFFFF: {										// EOF
					inFile.rewind_g();									// Rewind to chunk header
					inFile.ignore( chunk_size );						// Jump to next chunk
					inFile.peek();										// Peek for INI
					if ( inFile.eof() ) return;							// If EOF, return.
					i_Shape_INI( inFile ); has_INI = true;				// Otherwise, INI found
					return;
				}
				default:     {										// Invalid Opcode
					inFile.forget();									// Forget backup position
					return;
				}
			}
			inFile.rewind_g();									// Rewind to chunk header
			inFile.ignore( chunk_size );						// Jump to next chunk
		}
	}
	void shape::i_survey_Shape        ( const char* filename ) {
		util::fstream_B inFile ( filename, std::ios::in | std::ios::binary );
		CHECK_IF_OPENED( inFile, filename )
		
		uint32_t chunk_opcode, chunk_size, packet_count;		// Allocate vars
		while ( 1 ) {
			inFile.get_u32( chunk_opcode );						// Read chunk_opcode
			inFile.get_u32( chunk_size );						// Read chunk_size
			inFile.backup_g();									// Backup to chunk header
			inFile.get_u32( packet_count );						// Read packet_count
			switch ( chunk_opcode ) {							// Check for valid or EOF chunks
				case 0x0000:case 0x0110:
					printf( "opcode: %04X                        filepos: %6X\n",
						chunk_opcode,
						static_cast<uint32_t>( inFile.tellg() ) - 12
					);
					break;
				case 0x0010:case 0x0013:case 0x0011:case 0x0012:case 0x0018:case 0x0019:case 0x001A:case 0x001B:case 0x001C:case 0x001D:case 0x001E:case 0x001F:case 0x0020:case 0x0022:case 0x0030:case 0x0041:case 0x0040:case 0x0050:case 0x0060:case 0x0061:case 0x0080:case 0x0100:
					printf( "opcode: %04X     packets: %5u     filepos: %6X\n",
						chunk_opcode,
						packet_count,
						static_cast<uint32_t>( inFile.tellg() ) - 12
					);
					break;
				case 0xFFFF:
					goto end_of_file;
				default:
					goto illegal;
			}
			
			
			inFile.rewind_g();									// Rewind to chunk header
			inFile.ignore( chunk_size );						// Jump to next chunk
		}
		return;
		
		end_of_file:
			inFile.rewind_g();									// Rewind to chunk header
			inFile.ignore( chunk_size );						// Jump to next chunk
			inFile.peek();										// Peek for INI
			if ( inFile.eof() ) {								// If there is no more file
				return;												// return
			}
			has_INI = true;										// INI found
			printf( "INI Found        filepos: %6X\n",
				static_cast<uint32_t>( inFile.tellg() )
			);
			return;
			
		illegal:
			inFile.forget();									// Forget backup position
			printf( "ABORT!! ILLEGAL OPCODE!!\n" );				// Report error
			return;												// return
	}
	void shape::i_vtxPos_WavefrontObj ( const char* filename ) {
		util::fstream_B inFile ( filename, std::ios::in );
		CHECK_IF_OPENED( inFile, filename )
		
		char buffer[256];
		xyz temp;
		
		vtxPos.clear();
		while ( !inFile.eof() ) {		// Loop until file is empty
			inFile.getline( buffer );		// Copy line from file
			if ( !memcmp( buffer, "v ", 2 ) ) {
				sscanf( buffer, "v %f %f %f\n",	// Parse line
					&temp.x,						// X Position
					&temp.y,						// Y Position
					&temp.z							// Z Position
				);
				vtxPos.push_back( temp );		// Add to vtxPos chunk
			}
		}
		has_0010 = true;				// Inform has_0010
	}
	void shape::i_mapcode_CSV         ( const char* filename ) {
		util::fstream_B inFile ( filename, std::ios::in );
		CHECK_IF_OPENED( inFile, filename )
		
		char buffer[256];										// char* buffer
		uint32_t packet_count = 0;
		uint32_t attr, slip, bald;								// temp vars (can't pass bitfields by reference)
		while ( packet_count < colMesh.size() ) {
			printf( "%u   ", packet_count );
			if ( inFile.eof() ) {
				printf( "Warning!  CSV is smaller than expected!\n" );
				return;
			}
			inFile.getline( buffer );		// Copy line from file
			sscanf( buffer, "%u,%u,%u\n",	// Parse line
				&attr,
				&slip,
				&bald
			);
			printf( "%u\n", attr );
			colMesh[packet_count].mapcode.attribute = attr;
			colMesh[packet_count].mapcode.slip_code = slip;
			colMesh[packet_count].mapcode.is_bald   = bald;
			packet_count++;
			inFile.peek();					// Check for EOF
		}
		if ( !inFile.eof() ) {
			printf( "Warning!  CSV is larger than expected!\n" );
		}
	}
	void shape::i_ini                 ( const char* filename ) {
		util::fstream_B outFile ( filename, std::ios::out );	// Filestream
		CHECK_IF_OPENED( outFile, filename )
		
		i_Shape_INI( outFile );
	}
	void shape::o_Shape               ( const char* filename ) {
		util::fstream_B outFile ( filename, std::ios::out | std::ios::binary );	// Filestream
		CHECK_IF_OPENED( outFile, filename )
		
		if ( has_0000 ) o_Shape_0000( outFile );
		if ( has_0010 ) o_Shape_0010( outFile );
		if ( has_0013 ) o_Shape_0013( outFile );
		if ( has_0011 ) o_Shape_0011( outFile );
		if ( has_0012 ) o_Shape_0012( outFile );
		if ( has_0018 ) o_Shape_001X( outFile, 0 );
		if ( has_0019 ) o_Shape_001X( outFile, 1 );
		if ( has_001A ) o_Shape_001X( outFile, 2 );
		if ( has_001B ) o_Shape_001X( outFile, 3 );
		if ( has_001C ) o_Shape_001X( outFile, 4 );
		if ( has_001D ) o_Shape_001X( outFile, 5 );
		if ( has_001E ) o_Shape_001X( outFile, 6 );
		if ( has_001F ) o_Shape_001X( outFile, 7 );
		if ( has_0020 ) o_Shape_0020( outFile );
		if ( has_0022 ) o_Shape_0022( outFile );
		if ( has_0030 ) o_Shape_0030( outFile );
		if ( has_0041 ) o_Shape_0041( outFile );
		if ( has_0040 ) o_Shape_0040( outFile );
		if ( has_0050 ) o_Shape_0050( outFile );
		if ( has_0060 ) o_Shape_0060( outFile );
		if ( has_0061 ) o_Shape_0061( outFile );
		if ( has_0080 ) o_Shape_0080( outFile );
		if ( has_0100 ) o_Shape_0100( outFile );
		if ( has_0110 ) o_Shape_0110( outFile );
		                o_Shape_FFFF( outFile );
		if ( has_INI ) o_Shape_INI( outFile );
	}
	void shape::o_colMesh_WavefrontObj( const char* filename ) {
		util::fstream_B outFile ( filename, std::ios::out );	// Filestream
		CHECK_IF_OPENED( outFile, filename )
		
		char buffer[256];										// char* buffer
		sprintf( buffer, "o mesh0r\n" );	// Mesh Group
		for ( uint32_t packetCount = 0; packetCount < vtxPos.size(); packetCount++ ) {
			sprintf( buffer, "v %05f %05f %05f\n",	// Vertex Positions
				vtxPos[packetCount].x,
				vtxPos[packetCount].y,
				vtxPos[packetCount].z
			);
			outFile.write( buffer, strlen( buffer ) );
		}
		for ( uint32_t packetCount = 0; packetCount < colMesh.size(); packetCount++ ) {
			sprintf( buffer, "f %d %d %d\n",	// Faces
				colMesh[packetCount].index_a + 1,
				colMesh[packetCount].index_b + 1,
				colMesh[packetCount].index_c + 1
			);
			outFile.write( buffer, strlen( buffer ) );
		}
	}
	void shape::o_mapcode_CSV         ( const char* filename ) {
		util::fstream_B outFile ( filename, std::ios::out );	// Filestream
		CHECK_IF_OPENED( outFile, filename )
		
		char buffer[256];										// char* buffer
		for ( uint32_t packet_count = 0; packet_count < colMesh.size(); packet_count++ ) {
			sprintf( buffer, "%u,%u,%u\n",
				colMesh[packet_count].mapcode.attribute,
				colMesh[packet_count].mapcode.slip_code,
				colMesh[packet_count].mapcode.is_bald
			);
			outFile.write( buffer, strlen( buffer ) );
		}
	}
	void shape::o_ini                 ( const char* filename ) {
		util::fstream_B outFile ( filename, std::ios::out );	// Filestream
		CHECK_IF_OPENED( outFile, filename )
		
		o_Shape_INI( outFile );
	}
	void shape::m_colMesh_recalibrate (  ) {
		for ( uint32_t packet_count = 0; packet_count < colMesh.size(); packet_count++ ) {
			xyz& A = vtxPos[colMesh[packet_count].index_a];							// Lookup Vertex A
			xyz& B = vtxPos[colMesh[packet_count].index_b];							// Lookup Vertex B
			xyz& C = vtxPos[colMesh[packet_count].index_c];							// Lookup Vertex C
			xyz V ( (B.x - A.x), (B.y - A.y), (B.z - A.z) );						// V is vector AB, shifted to start at origin
			xyz W ( (C.x - A.x), (C.y - A.y), (C.z - A.z) );						// W is vector AC, shifted to start at origin
			colMesh[packet_count].tri_normal.x = ( V.y * W.z ) - ( V.z * W.y );		// Cross Product X (solving surface normal)
			colMesh[packet_count].tri_normal.y = -(( V.x * W.z ) - ( V.z * W.x ));	// Cross Product Y (solving surface normal)
			colMesh[packet_count].tri_normal.z = ( V.x * W.y ) - ( V.y * W.x );		// Cross Product Z (solving surface normal)
			colMesh[packet_count].tri_normal.normalize();							// Normalize       (solving surface normal)
			xyz middle (															// Middle is the average of A, B, and C.
				( A.x + B.x + C.x ) / 3,	// X
				( A.y + B.y + C.y ) / 3,	// Y
				( A.z + B.z + C.z ) / 3		// Z
			);
			colMesh[packet_count].magnitude = (										// Magnitude is a value used in collision math
				( colMesh[packet_count].tri_normal.x * middle.x ) +
				( colMesh[packet_count].tri_normal.y * middle.y ) +
				( colMesh[packet_count].tri_normal.z * middle.z )
			);
		}
	}
	void shape::m_survey_selfcheck    (  ) {
		printf( "opcode: 0000                        %u\n", has_0000 );
		printf( "opcode: 0010     packets: %5u     %u\n", vtxPos.size(),      has_0010 );
		printf( "opcode: 0013     packets: %5u     %u\n", vtxColor.size(),    has_0013 );
		printf( "opcode: 0011     packets: %5u     %u\n", vtxNrm.size(),      has_0011 );
		printf( "opcode: 0012     packets: %5u     %u\n", vtxNBT.size(),      has_0012 );
		printf( "opcode: 0018     packets: %5u     %u\n", texCoord[0].size(), has_0018 );
		printf( "opcode: 0019     packets: %5u     %u\n", texCoord[1].size(), has_0019 );
		printf( "opcode: 001A     packets: %5u     %u\n", texCoord[2].size(), has_001A );
		printf( "opcode: 001B     packets: %5u     %u\n", texCoord[3].size(), has_001B );
		printf( "opcode: 001C     packets: %5u     %u\n", texCoord[4].size(), has_001C );
		printf( "opcode: 001D     packets: %5u     %u\n", texCoord[5].size(), has_001D );
		printf( "opcode: 001E     packets: %5u     %u\n", texCoord[6].size(), has_001E );
		printf( "opcode: 001F     packets: %5u     %u\n", texCoord[7].size(), has_001F );
		printf( "opcode: 0020     packets: %5u     %u\n", texImg.size(),      has_0020 );
		printf( "opcode: 0022     packets: %5u     %u\n", texAttr.size(),     has_0022 );
		printf( "opcode: 0030     packets: %5u     %u\n", material.size(),    has_0030 );
//		printf( "opcode: 0041     packets: %5u     %u\n", mtxEvl.size(),      has_0041 );
//		printf( "opcode: 0040     packets: %5u     %u\n", mtx.size(),         has_0040 );
//		printf( "opcode: 0050     packets: %5u     %u\n", mesh.size(),        has_0050 );
//		printf( "opcode: 0060     packets: %5u     %u\n", rig.size(),         has_0060 );
//		printf( "opcode: 0061     packets: %5u     %u\n", rigName.size(),     has_0061 );
//		printf( "opcode: 0080     packets: %5u     %u\n", ???.size(),         has_0080 );
		printf( "opcode: 0100     packets: %5u     %u\n", colMesh.size(),     has_0100 );
		printf( "opcode: 0110                        %u\n", has_0110 );
		printf( "INI                                 %u\n", has_INI );
	}

	/// misc C functions
	xyz calculateNormal( const xyz& A, const xyz& B, const xyz& C ) {
		xyz V ( (B.x - A.x), (B.y - A.y), (B.z - A.z) );	// Shift vector to start on origin
		xyz W ( (C.x - A.x), (C.y - A.y), (C.z - A.z) );	// Shift vector to start on origin
		xyz result;
		result.x = ( V.y * W.z ) - ( V.z * W.y );	// Cross Product X
		result.y = ( V.z * W.x ) - ( V.x * W.y );	// Cross Product Y
		result.z = ( V.x * W.y ) - ( V.y * W.x );	// Cross Product Z
		result.normalize();
		return result;
	}
	uint32_t calculate_chunk_size( uint32_t packetCount, uint32_t packet_size ) {
		return ( packetCount * packet_size ) + ( ( 0x20 - ( ( packetCount * packet_size ) % 0x20 ) ) % 0x20 ) + 0x18;
	}
}
