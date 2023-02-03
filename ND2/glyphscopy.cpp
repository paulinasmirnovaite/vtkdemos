
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkCamera.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include <vtkDataSetReader.h>
#include <vtkDataSetMapper.h>
#include <vtkPolyDataReader.h>
#include "vtkPointData.h"
#include "vtkUnstructuredGrid.h"
#include <vtkScalarBarActor.h>
#include <vtkPointSource.h>
#include <vtkConeSource.h>
#include "vtkProperty.h"
#include <vtkGlyph3D.h>

int main() {
	//
	
	//vtkDataSetReader: nuskaitomas duomenų rinkinys

	vtkDataSetReader *read = vtkDataSetReader::New();
	read->SetFileName("C:/VTKtest/ND2/bunkeris.vtk");
	read->Update();
	read->GetOutput()->Print(std::cout);
	//
	//
	vtkDataSetMapper *mapper = vtkDataSetMapper::New();
	mapper->SetScalarModeToUsePointFieldData();
	mapper->ScalarVisibilityOff();
	mapper->SetInputConnection(read->GetOutputPort());
	//
	vtkActor*actor = vtkActor::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->SetOpacity(0.4);
	//
	// vtkConeSource: sukuriamas kūgio objektas - vtkGlyph3D šaltinis
	vtkConeSource *cone = vtkConeSource::New();
	cone->SetResolution(12);
	//
	// vtkGlyph3D: sukuriamas kûgio objektas - vtkGlyph3D šaltinis
	vtkGlyph3D *glyph = vtkGlyph3D::New();
	glyph->SetSourceConnection(cone->GetOutputPort());
	glyph->SetScaleModeToScaleByScalar();
	glyph->SetInputArrayToProcess(vtkDataSetAttributes::SCALARS, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, "common/Scalar4");
	glyph->Update();
	//
	// vtkPolyDataMapper: sukuriamas glyph'ų mapperis.
	// Spalvinimas pagal skaliarą(common/Scalar4)
	vtkPolyDataMapper *glMapper = vtkPolyDataMapper::New();
	glMapper->ColorByArrayComponent("common/Scalar4", 0);
	glMapper->SetScalarRange(read->GetOutput()->GetPointData()->GetArray("common/Scalar4")->GetRange(0));
	glMapper->SetScalarModeToUsePointFieldData();
	glMapper->SetInputConnection(glyph->GetOutputPort());
	//
	// vtkActor: sukuriamas glyph'ų aktorius, prie kurio prijungiamas mapperis
	vtkActor *glActor = vtkActor::New();
	glActor->SetMapper(glMapper);

	//
	// vtkActor: glyphams sukuriamas spalvų skalės aktorius 
	vtkScalarBarActor* scalarBar = vtkScalarBarActor::New();
	scalarBar->SetLookupTable(glMapper->GetLookupTable());
	scalarBar->SetTitle("AirTemp");
	scalarBar->SetNumberOfLabels(5);

	//
	// Create the Renderer and assign actors to it. A renderer is like a
	// viewport. It is part or all of a window on the screen and it is
	// responsible for drawing the actors it has.  
	//
	// vtkRenderer: sukuriamas renderis, prie kurio prijungiami aktoriai
	vtkRenderer *renderer = vtkRenderer::New();
	renderer->AddActor(actor);
	renderer->AddActor(glActor);
	renderer->AddActor2D(scalarBar);
	renderer->SetBackground(0.11, 0.1, 0.1);

	//
	// Finally we create the render window which will show up on the screen.
	// We put our renderer into the render window using AddRenderer. We also
	// set the size to be 300 pixels by 300.
	//
	// vtkRenderWindow: sukuriamas atvaizdavimo langas, prie kurio prijungiamas renderis
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	renWin->AddRenderer(renderer);
	renWin->SetSize(900, 900);

	//
	// The vtkRenderWindowInteractor class watches for events (e.g., keypress,
	// mouse) in the vtkRenderWindow. These events are translated into
	// event invocations that VTK understands (see VTK/Common/vtkCommand.h
	// for all events that VTK processes). Then observers of these VTK
	// events can process them as appropriate.
	//
	// vtkRenderWindowInteractor: sukuriamas interaktoriaus objektas, prie kurio prijungiams atvaizdavimo langas
	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);

	//
	// By default the vtkRenderWindowInteractor instantiates an instance
	// of vtkInteractorStyle. vtkInteractorStyle translates a set of events
	// it observes into operations on the camera, actors, and/or properties
	// in the vtkRenderWindow associated with the vtkRenderWinodwInteractor.
	// Here we specify a particular interactor style.
	//
	// vtkInteractorStyleTrackballCamera: sukuriamas interaktyvumo stiliaus TrackballCamera objektas
	vtkInteractorStyleTrackballCamera *style = vtkInteractorStyleTrackballCamera::New();
	iren->SetInteractorStyle(style);

	//
	// Interact with data
	//	
	// vtkRenderWindowInteractor: inicializacija ir paleidimas (vietoje atvaizdavimo ciklo)
	iren->Initialize();
	iren->Start();

	//
	// Free up any objects we created. All instances in VTK are deleted by
	// using the Delete() method.
	//
	// Naikinami nereikalingi objektai, atlaisvinama atmintis
	renderer->Delete();
	renWin->Delete();
	iren->Delete();
	actor->Delete();
	glMapper->Delete();
	mapper->Delete();
	glActor->Delete();
	scalarBar->Delete();
	glyph->Delete();
	cone->Delete();
	
	read->Delete();

	return EXIT_SUCCESS;
}