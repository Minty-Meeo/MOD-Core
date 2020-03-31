#include "util.h"
#include "shape.h"

#include <iostream>

void report_args( char** argv, int32_t num ) {
	printf( "Errand:" );
	for ( int32_t i = 0; i < num; i++ )
		printf( "\t%s", argv[i] );
	printf( "\n" );
}

int main( int32_t argc, char* argv[] ) {
//	if ( argc == 1 || !strcmp( argv[1], "/?" ) ) { printHelp();		return -1; }	// Triggers for help popup
	model3D::shape mainShape;
	
	for ( int32_t i = 1; i < argc; i++ ) {	// Parse errands
		/**/ if ( !strcmp( argv[i], "i_Shape" )               ) { report_args( &argv[i], 2 ); mainShape.i_Shape               ( argv[i+1] ); i++; }
		else if ( !strcmp( argv[i], "i_vtxPos_WavefrontObj" ) ) { report_args( &argv[i], 2 ); mainShape.i_vtxPos_WavefrontObj ( argv[i+1] ); i++; }
		else if ( !strcmp( argv[i], "i_survey_Shape" )        ) { report_args( &argv[i], 2 ); mainShape.i_survey_Shape        ( argv[i+1] ); i++; }
		else if ( !strcmp( argv[i], "i_mapcode_CSV" )         ) { report_args( &argv[i], 2 ); mainShape.i_mapcode_CSV         ( argv[i+1] ); i++; }
		else if ( !strcmp( argv[i], "i_ini" )                 ) { report_args( &argv[i], 2 ); mainShape.i_ini                 ( argv[i+1] ); i++; }
		else if ( !strcmp( argv[i], "o_colMesh_WavefrontObj" )) { report_args( &argv[i], 2 ); mainShape.o_colMesh_WavefrontObj( argv[i+1] ); i++; }
		else if ( !strcmp( argv[i], "o_Shape" )               ) { report_args( &argv[i], 2 ); mainShape.o_Shape               ( argv[i+1] ); i++; }
		else if ( !strcmp( argv[i], "o_mapcode_CSV" )         ) { report_args( &argv[i], 2 ); mainShape.o_mapcode_CSV         ( argv[i+1] ); i++; }
		else if ( !strcmp( argv[i], "o_ini" )                 ) { report_args( &argv[i], 2 ); mainShape.o_ini                 ( argv[i+1] ); i++; }
		else if ( !strcmp( argv[i], "m_colMesh_recalibrate" ) ) { report_args( &argv[i], 1 ); mainShape.m_colMesh_recalibrate (           ); i;   }
		else if ( !strcmp( argv[i], "m_survey_selfcheck" )    ) { report_args( &argv[i], 1 ); mainShape.m_survey_selfcheck    (           ); i;   }
		else                                                    { report_args( &argv[i], 1 ); printf( "INVALID ERRAND!\n" )                ; i;   }
	}
}
