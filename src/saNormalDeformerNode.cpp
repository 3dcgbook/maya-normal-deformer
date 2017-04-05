
#include "saNormalDeformerNode.h"

MTypeId     saNormalDeformer::id(0x00333);
MObject     saNormalDeformer::aScale;
MObject     saNormalDeformer::aReverse;
MObject     saNormalDeformer::aParallel;

saNormalDeformer::saNormalDeformer() {}
saNormalDeformer::~saNormalDeformer() {}

MStatus saNormalDeformer::GetInputMesh(MDataBlock& data, unsigned int geomIndex, MObject& oInputMesh)
{
	MStatus status;
	MArrayDataHandle hInput = data.outputArrayValue(input, &status);
	status = hInput.jumpToElement(geomIndex);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	MDataHandle hInputGeom = hInput.outputValue().child(inputGeom);
	oInputMesh = hInputGeom.asMesh();
	return status;
}

MStatus saNormalDeformer::deform(MDataBlock& data, MItGeometry& iter, const MMatrix& mat, unsigned int geomIndex)

{
	MStatus status;

	MThreadUtils::syncNumOpenMPThreads();

	float env = data.inputValue(envelope).asFloat();
	bool reverseFlag = data.inputValue(aReverse).asBool();
	bool parallelFlag = data.inputValue(aParallel).asBool();
	double scale = data.inputValue(aScale).asDouble();
	double reverse = 1.0;
	
	if (reverseFlag == true)
		reverse = -1.0;

	MObject oInputMesh;
	status = GetInputMesh(data, geomIndex, oInputMesh);

	MItMeshVertex iterMv(oInputMesh);
	MFnMesh fnMesh(oInputMesh);
	MPointArray allPt;
	MVectorArray allNrms;
	unsigned int count;
	count = fnMesh.numVertices();
	
	fnMesh.getPoints(allPt, MSpace::kObject);

	MVector nrm;
	
#ifdef _OPENMP
#pragma omp parallel for
#endif
	for (unsigned int i = 0; i < count; i++){
		iterMv.getNormal(nrm, MSpace::kObject);
		allPt[i] += (nrm * weightValue(data, geomIndex, i) * scale * env * reverse);
		iterMv.next();
	}
		
	iter.setAllPositions(allPt);

	return MS::kSuccess;
}

void* saNormalDeformer::creator()
{
	return new saNormalDeformer();
}

MStatus saNormalDeformer::initialize()

{
	MFnNumericAttribute nAttr;
	MStatus				stat;

	aParallel = nAttr.create("parallel", "pl", MFnNumericData::kBoolean, true, &stat);
	nAttr.setKeyable(true);
	addAttribute(aParallel);
	aReverse = nAttr.create("reverse", "rev", MFnNumericData::kBoolean, false, &stat);
	nAttr.setKeyable(true);
	addAttribute(aReverse);
	aScale = nAttr.create("scale", "s", MFnNumericData::kDouble, 0.0, &stat);
	nAttr.setKeyable(true);
	addAttribute(aScale);
	

	attributeAffects(aParallel, outputGeom);
	attributeAffects(aReverse, outputGeom);
	attributeAffects(aScale, outputGeom);


	MGlobal::executeCommand("makePaintable -attrType multiFloat -sm deformer saNormalDeformer weights");
	return MS::kSuccess;

}

