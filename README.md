# MOD-Core
A new tool to internalize, manipulate, and create Shape files (*.mod) from Pikmin 1.

### INPUT
* ``i_Shape`` +1 argument<br>
Import Shape file (*.mod).

* ``i_vtxPos_WavefrontObj`` +1 argument<br>
Import vertex positions from Wavefront Obj file (*.obj).

* ``i_survey_Shape`` +1 argument<br>
Scan Shape file (*.mod) without importing.

* ``i_mapcode_CSV`` +1 argument<br>
Import mapcode data to 0100 chunk from a CSV file<br>
	\- column 1 is **attribute**<br>
	\- column 2 is **slip_code**<br>
	\- column 3 is **is_bald**<br>

### OUTPUT
* ``o_colMesh_WavefrontObj`` +1 argument<br>
Export collision mesh to Wavefront Obj file (*.obj).

* ``o_Shape`` +1 argument<br>
Export Shape file (*.mod)

* ``o_mapcode_CSV`` +1 argument<br>
Export mapcode data from 0100 chunk from a CSV file<br>
	\- column 1 is **attribute**<br>
	\- column 2 is **slip_code**<br>
	\- column 3 is **is_bald**<br>

### MANIPULATE
* ``m_colMesh_recalibrate``<br>
Recalculate the collision mesh's tris' surface normal and magnitude.

* ``m_survey_selfcheck``<br>
Scan internalized data (useful for debugging/development)
