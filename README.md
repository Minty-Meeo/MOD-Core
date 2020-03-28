# MOD-Core
A new tool to internalize, manipulate, and create Shape files (*.mod) from Pikmin 1.

## USAGE
MOD-Core is a command line utility that takes any number of "errands" as arguments.  These errands can be strung together to perform several tasks in sequence.
### EXAMPLES
`MOD-Core i_Shape "naka13.mod"`<br>
	1) Input Shape file (\*.mod) `"naka13.mod"`

`MOD-Core i_Shape "forest.mod" o_colMesh_WavefrontObj "forestCol.obj"`<br>
	1) Input Shape file (\*.mod) `"forest.mod"`<br>
	2) Output Wavefront Object file (\*.obj) `"forestCol.obj"`

`MOD-Core i_Shape "forest.mod" i_vtxPos_WavefrontObj "untitled.obj" o_Shape "newforest.mod"`<br>
	1) Input Shape file (\*.mod) `"forest.mod"`<br>
	2) Input Wavefront Object file (\*.obj) `"untitled.obj"` (only the vertex positions though)<br>
	3) Output Shape file (\*.mod) `"newforest.mod"`


## ERRANDS
### INPUT
* `i_Shape <filepath>`<br>
Import Shape file (\*.mod) designated by `<filepath>`.

* `i_vtxPos_WavefrontObj <filepath>`<br>
Import vertex positions from Wavefront Obj file (\*.obj) designated by `<filepath>`.

* `i_survey_Shape <filepath>`<br>
Scan Shape file (\*.mod) designated by `<filepath>` without importing.

* `i_mapcode_CSV <filepath>`<br>
Import mapcode data to the collision mesh from a Comma Seperated Values file (\*.csv) file designated by `<filepath>`.<br>
	\- Column 1 is **attribute**<br>
	\- Column 2 is **slip_code**<br>
	\- Column 3 is **is_bald**<br>

### OUTPUT
* `o_colMesh_WavefrontObj <filepath>`<br>
Export collision mesh to Wavefront Obj file (\*.obj) designated by `<filepath>`.

* `o_Shape <filepath>`<br>
Export Shape file (\*.mod) designated by `<filepath>`.

* `o_mapcode_CSV <filepath>`<br>
Export mapcode data from the collision mesh to a Comma Seperated Values file (\*.csv) designated by `<filepath>`.<br>
	\- Column 1 is **attribute**<br>
	\- Column 2 is **slip_code**<br>
	\- Column 3 is **is_bald**<br>

### MANIPULATE
* `m_colMesh_recalibrate`<br>
Recalculate the collision mesh's tris' surface normal and magnitude.

* `m_survey_selfcheck`<br>
Scan internalized data (useful for debugging/development)
