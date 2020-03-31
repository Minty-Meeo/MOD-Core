#ifndef shape_h
#define shape_h

#include "util.h"
#include <vector>
#include <stdint.h>
#include <fstream>
#include <math.h>
#include <string.h>
#include <array>

namespace model3D {
	
	struct xyz {
		float x;
		float y;
		float z;
		
		void normalize();
		xyz() {
		}
		xyz( float a, float b, float c ) {
			x = a;
			y = b;
			z = c;
		}
	};
/*	struct rgba {
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;
		operator uint32_t() const {
			return ( r << 24 ) |
			       ( g << 16 ) |
			       ( b <<  8 ) |
			       ( a       );
		}
		rgba& operator= ( uint32_t val ) {
			r = ( val & 0xFF000000 ) >> 24;
			g = ( val & 0x00FF0000 ) >> 16;
			b = ( val & 0x0000FF00 ) >>  8;
			a = ( val & 0x000000FF )      ;
			return *this;
		}
	};*/
	union color {
		uint32_t rgba;
		uint8_t channel[4];
		#if LITTLE_ENDIAN
		enum {
			r = 3,
			g = 2,
			b = 1,
			a = 0,
		};
		#elif // BIG_ENDIAN
		enum {
			r = 0,
			g = 1,
			b = 2,
			a = 3,
		};
		#endif
		color(  ) {
		}
		color( uint32_t value ) {
			rgba = value;
		}
		color( uint8_t r1, uint8_t g1, uint8_t b1, uint8_t a1 ) {
			channel[r] = r1;
			channel[g] = g1;
			channel[b] = b1;
			channel[a] = a1;
		}
		color& operator= ( uint32_t value ) {
			rgba = value;
			return *this;
		}
	};
/*	struct rgba {
		uint32_t hue;
		uint8_t* r() { return reinterpret_cast<uint8_t*>(&hue)+3; }	// Access red channel
		uint8_t* g() { return reinterpret_cast<uint8_t*>(&hue)+2; }	// Access green channel
		uint8_t* b() { return reinterpret_cast<uint8_t*>(&hue)+1; }	// Access blue channel
		uint8_t* a() { return reinterpret_cast<uint8_t*>(&hue)  ; }	// Access alpha channel
	};*/
	struct nbt {
		xyz normal;
		xyz binormal;
		xyz tangent;
	};
	struct uv {
		float u;
		float v;
		
		uv() {
		}
		uv( float a, float b ) {
			u = a;
			v = b;
		}
	};
	struct mTXE {
		enum GX_FMT : uint32_t {
			RGB565 = 0,
			CMPR   = 1,
			RGB5A3 = 2,
			I4     = 3,
			I8     = 4,
			IA4    = 5,
			IA8    = 6,
			RGBA8  = 7,
		};
		uint16_t res_x;
		uint16_t res_y;
		uint32_t format;
		std::vector<char> data;
	};
	struct TXE {
		enum GX_FMT : uint16_t {
			RGB565,
			CMPR,
			RGB5A3,
			I4,
			I8,
			IA4,
			IA8,
			RGBA8,
		};
		uint16_t size_x;
		uint16_t size_y;
		GX_FMT format;
		std::string data;
	};
	struct attr {
		uint16_t texture_index;
		uint16_t pallette_index;
		uint8_t wrap_s;
		uint8_t wrap_t;
		uint8_t has_embossbump;
		uint8_t has_mipmap;
		float LOD;
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
				util::setField( value, attribute, 0, 3 );
				util::setField( value, slip_code, 3, 2 );
				util::setField( value, is_bald  , 5, 1 );
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
		void i_Shape_INI ( util::fstream_B& );
		
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
		struct {	// "has" bitfield
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
		struct { // info                 // 0000
			uint16_t year;
			uint8_t month;
			uint8_t date;
			struct { // bitfield
				unsigned scalingrule : 1;
//				unsigned ?           : 1;
//				unsigned ?           : 1;
				unsigned embossbump  : 1;
//				unsigned ?           : 1;
			};
		} info;
		std::vector<xyz>         vtxPos;      // 0010
		std::vector<color>       vtxColor;    // 0013
		std::vector<xyz>         vtxNrm;      // 0011
		std::vector<nbt>         vtxNBT;      // 0012
		std::vector<uv>          texCoord[8]; // 0018 to 001F
		std::vector<mTXE>        texImg;      // 0020
		std::vector<attr>        texAttr;     // 0022
		std::vector<mat>         material;    // 0030
//		std::vector< ? >         mtxEvl;      // 0041
//		std::vector< ? >         mtx;         // 0040
//		std::vector< ? >         mesh;        // 0050
//		std::vector< ? >         joint;       // 0060
		std::vector<std::string> jointName;   // 0061
//		std::vector< ? >         ???          // 0080
		std::vector<colTri>      colMesh;     // 0100
		std::string              ini;         // INI
		
		void i_Shape               ( const char* );
		void i_vtxPos_WavefrontObj ( const char* );
		void i_survey_Shape        ( const char* );
		void i_mapcode_CSV         ( const char* );
		void i_ini                 ( const char* );
		void o_Shape               ( const char* );
		void o_colMesh_WavefrontObj( const char* );
		void o_mapcode_CSV         ( const char* );
		void o_ini                 ( const char* );
		void m_colMesh_recalibrate (  );
		void m_survey_selfcheck    (  );
		

		shape() {
		}
		shape( const char* str ) {
			i_Shape( str );
		}
	};
	
	xyz calculateNormal( const xyz&, const xyz&, const xyz& );
	uint32_t calculate_chunk_size( uint32_t, uint32_t );
}

#endif
