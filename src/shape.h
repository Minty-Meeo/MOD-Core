#ifndef shape_h
#define shape_h

#include "util.h"
#include <vector>
#include <stdint.h>
#include <fstream>
#include <math.h>
#include <string.h>

namespace mod {
	
	struct hdr {
		uint16_t year;
		uint8_t month;
		uint8_t date;
	};
	struct xy {
		float x;
		float y;
		
		void normalize();
		xy();
		xy( float, float );
	};
	struct xyz {
		float x;
		float y;
		float z;
		
		void normalize();
		xyz();
		xyz( float, float, float );
	};
	struct nbt {
		xyz normal;
		xyz binormal;
		xyz tangent;
	};
	struct img {
	
	};
	struct attr {
		uint16_t texture_index;
		uint16_t pallette_index;
		uint8_t wrap_s;
		uint8_t wrap_t;
	};
	struct mat {
		
	};
	struct colTri {
		struct { // mapcode
			unsigned attribute : 3;
			unsigned slip_code : 2;
			unsigned is_bald   : 1;
			uint32_t u32(  ) {
				uint32_t value = 0;
				value |= attribute << (32-3);
				value |= slip_code << (32-3-2);
				value |= is_bald   << (32-3-2-1);
				return value;
			}
		} mapcode;
		uint32_t index_a;
		uint32_t index_b;
		uint32_t index_c;
		int16_t neighbor_ab;
		int16_t neighbor_bc;
		int16_t neighbor_ca;
		xyz tri_normal;
		float magnitude;
	public:
		void recalibrate( std::vector<xyz>& );
	};
	
	class shape {
	private:
		/// i_Shape private subfunctions
		void i_Shape_0000( util::fstream_B& );
		void i_Shape_0010( util::fstream_B& );
		void i_Shape_0013( util::fstream_B& );
		void i_Shape_0011( util::fstream_B& );
		void i_Shape_0012( util::fstream_B& );
		void i_Shape_001X( util::fstream_B&, uint32_t );
		void i_Shape_0020( util::fstream_B& );
		void i_Shape_0022( util::fstream_B& );
		void i_Shape_0030( util::fstream_B& );
		void i_Shape_0041( util::fstream_B& );
		void i_Shape_0040( util::fstream_B& );
		void i_Shape_0050( util::fstream_B& );
		void i_Shape_0060( util::fstream_B& );
		void i_Shape_0061( util::fstream_B& );
		void i_Shape_0080( util::fstream_B& );
		void i_Shape_0100( util::fstream_B& );
		void i_Shape_0110( util::fstream_B& );
		
		/// o_Shape private subfunctions
		void o_Shape_0000( util::fstream_B& );
		void o_Shape_0010( util::fstream_B& );
		void o_Shape_0013( util::fstream_B& );
		void o_Shape_0011( util::fstream_B& );
		void o_Shape_0012( util::fstream_B& );
		void o_Shape_001X( util::fstream_B&, uint32_t );
		void o_Shape_0020( util::fstream_B& );
		void o_Shape_0022( util::fstream_B& );
		void o_Shape_0030( util::fstream_B& );
		void o_Shape_0041( util::fstream_B& );
		void o_Shape_0040( util::fstream_B& );
		void o_Shape_0050( util::fstream_B& );
		void o_Shape_0060( util::fstream_B& );
		void o_Shape_0061( util::fstream_B& );
		void o_Shape_0080( util::fstream_B& );
		void o_Shape_0100( util::fstream_B& );
		void o_Shape_0110( util::fstream_B& );
		void o_Shape_FFFF( util::fstream_B& );
		void o_Shape_INI( util::fstream_B& );
	
	public:
		struct {	// "has flags" bitfield
			unsigned has_0000 : 1;
			unsigned has_0010 : 1;
			unsigned has_0013 : 1;
			unsigned has_0011 : 1;
			unsigned has_0012 : 1;
			unsigned has_0018 : 1;
			unsigned has_0019 : 1;
			unsigned has_001A : 1;
			unsigned has_001B : 1;
			unsigned has_001C : 1;
			unsigned has_001D : 1;
			unsigned has_001E : 1;
			unsigned has_001F : 1;
			unsigned has_0020 : 1;
			unsigned has_0022 : 1;
			unsigned has_0030 : 1;
			unsigned has_0041 : 1;
			unsigned has_0040 : 1;
			unsigned has_0050 : 1;
			unsigned has_0060 : 1;
			unsigned has_0061 : 1;
			unsigned has_0080 : 1;
			unsigned has_0100 : 1;
			unsigned has_0110 : 1;
			unsigned has_INI  : 1;
		};
		hdr					header;      // 0000
		std::vector<xyz>    vtxPos;      // 0010
//		std::vector< ? >    vtxColor;    // 0013
		std::vector<xyz>    vtxNrm;      // 0011
		std::vector<nbt>    vtxNBT;      // 0012
		std::vector<xy>     texCoord[8]; // 0018 to 001F
		std::vector<img>    texImg;      // 0020
		std::vector<attr>   texAttr;     // 0022
		std::vector<mat>    material;    // 0030
//		std::vector< ? >    mtxEvl;      // 0041
//		std::vector< ? >    mtx;         // 0040
//		std::vector< ? >    mesh;        // 0050
//		std::vector< ? >    rig;         // 0060
//		std::vector< ? >    ???          // 0061
//		std::vector< ? >    ???          // 0080
		std::vector<colTri> colMesh;     // 0100
		
		void i_Shape               ( const char* );
		void i_vtxPos_WavefrontObj ( const char* );
		void i_survey_Shape        ( const char* );
		void i_mapcode_CSV         ( const char* );
		void o_Shape               ( const char* );
		void o_colMesh_WavefrontObj( const char* );
		void o_mapcode_CSV         ( const char* );
		void m_colMesh_recalibrate (  );
		void m_survey_selfcheck    (  );
		

		shape();
		shape( const char* );
	};
	
	xyz calculateNormal( const xyz&, const xyz&, const xyz& );
	uint32_t calculate_chunk_size( uint32_t, uint32_t );
}

#endif
