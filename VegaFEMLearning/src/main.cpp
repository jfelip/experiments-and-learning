
//VEGA FEM INCLUDES
#include "volumetricMeshLoader.h"
#include "corotationalLinearFEM.h"
#include "corotationalLinearFEMForceModel.h"
#include "generateMassMatrix.h"
#include "implicitBackwardEulerSparse.h"

int main(int argc, char** argv)
{

	//Load a volumetric mesh from file
	char inputFilename[200] = "/home/jfelip/Installations/VegaFEM-v3.1/models/turtle/turtle-volumetric-heterogeneous.veg";
	VolumetricMesh * volumetricMesh = VolumetricMeshLoader::load(inputFilename);
	if (volumetricMesh == NULL)
		printf("Error: failed to load mesh.\n");
	else
		printf("Success. Number of vertices: %d . Number of elements: %d .\n",

	volumetricMesh->getNumVertices(), volumetricMesh->getNumElements());


	//Convert the mesh to a TetMesh
	TetMesh * tetMesh;
	if (volumetricMesh->getElementType() == VolumetricMesh::TET)
		tetMesh = (TetMesh*) volumetricMesh; // such down-casting is safe in Vega
	else
	{
		printf("Error: not a tet mesh.\n");
		exit(1);
	}

	CorotationalLinearFEM * deformableModel = new CorotationalLinearFEM(tetMesh);

	// create the class to connect the deformable model to the integrator
	ForceModel * forceModel = new CorotationalLinearFEMForceModel(deformableModel);
	int r = 3 * tetMesh->getNumVertices(); // total number of DOFs
	double timestep = 0.0333; // the timestep, in seconds
	SparseMatrix * massMatrix;
	// create consistent (non-lumped) mass matrix
	GenerateMassMatrix::computeMassMatrix(tetMesh, &massMatrix, true);
	// constraining vertices 4, 10, 14 (constrained DOFs are specified 0-indexed):
	int numConstrainedDOFs = 9;
	int constrainedDOFs[9] = { 12, 13, 14, 30, 31, 32, 42, 43, 44 };
	// (tangential) Rayleigh damping
	double dampingMassCoef = 0.0; // "underwater"-like damping (here turned off)
	double dampingStiffnessCoef = 0.01; // (primarily) high-frequency damping

	// initialize the integrator
	ImplicitBackwardEulerSparse * implicitBackwardEulerSparse = new
	ImplicitBackwardEulerSparse(r, timestep, massMatrix, forceModel,numConstrainedDOFs, constrainedDOFs,dampingMassCoef, dampingStiffnessCoef);


	// allocate buffer for external forces
	double * f = (double*) malloc (sizeof(double) * r);
	int numTimesteps = 1000;
	for(int i=0; i<numTimesteps; i++)
	{
		// important: must always clear forces, as they remain in effect unless changed
		implicitBackwardEulerSparse->SetExternalForcesToZero();
		if (i == 0) // set some force at the first timestep
		{
			for(int j=0; j<r; j++)
				f[j] = 0; // clear to 0
			f[37] = -500; // apply force of -500 N to vertex 12, in y-direction, 3*12+1 = 37
			implicitBackwardEulerSparse->SetExternalForces(f);
		}
		implicitBackwardEulerSparse->DoTimestep();
	}

	// allocate buffer to read the resulting displacements
	double * u = (double*) malloc (sizeof(double) * r);
	implicitBackwardEulerSparse->GetqState(u);


	//Cleanup
	delete implicitBackwardEulerSparse;
	delete massMatrix;
	delete forceModel;
	delete volumetricMesh;
	free(u);
	free(f);

	printf("Exit 0\n");
	return 0;
}
