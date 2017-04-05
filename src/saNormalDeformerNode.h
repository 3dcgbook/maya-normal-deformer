#ifndef _saNormalDeformerNode
#define _saNormalDeformerNode

#include <maya/MPxDeformerNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MTypeId.h> 
#include <maya/MItGeometry.h>
#include <maya/MDataBlock.h>
#include <maya/MMatrix.h>
#include <maya/MItMeshVertex.h>
#include <maya/MPointArray.h>
#include <maya/MVectorArray.h>
#include <maya/MFnMesh.h>
#include <maya/MGlobal.h>
#include <maya/MTimer.h>
#include <maya/MThreadUtils.h>


class saNormalDeformer : public MPxDeformerNode
{
public:
						saNormalDeformer();
	virtual				~saNormalDeformer(); 

	virtual MStatus		deform( MDataBlock& data, MItGeometry& iter, const MMatrix& mat, unsigned int geomIndex);

	static  void*		creator();
	static  MStatus		initialize();

	static	MTypeId		id;
	static MObject aReverse;
	static MObject aScale;
	static MObject aParallel;

private:
	MStatus GetInputMesh(MDataBlock& data, unsigned int geomIndex, MObject& oInputMesh);
};

#endif
