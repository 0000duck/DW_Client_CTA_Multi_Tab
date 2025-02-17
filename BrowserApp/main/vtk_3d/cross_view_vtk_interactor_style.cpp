/*
 *  
 *  $Id: crossviewvtkinteractor.cpp $
 *  DeepWise 
 *
 *  Copyright 2017-2030 DeepWise All rights reserved.
 *  http://www.deepwise.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 *  Code based in VTK
 */

#include "stdafx.h"
#include "cross_view_vtk_interactor_style.h"
#include "left_down_vtk_actor.h"
#include "cross_view_vtk_actor.h"

#pragma once
#ifdef __DEPRECATED
#undef __DEPRECATED
#endif 

vtkStandardNewMacro(CrossViewVtkInteractorStyle)

void CrossViewVtkInteractorStyle::SetImageActor(CrossViewVtkActorBase* actor) {
    //_ImageActor = image_actor;
    m_actor = actor;
}
void CrossViewVtkInteractorStyle::SetDicomImageReader(vtkDICOMImageReader* dicomReader)
{
    _DicomReader = dicomReader;
}

void CrossViewVtkInteractorStyle::SetSlice(int slice)
{
    this->Modified();

    int eventStation[3] = { 0 };
    this->GetInteractor()->GetEventPosition(eventStation);
    
    m_actor->SetSlice(slice); 
}

void CrossViewVtkInteractorStyle::MoveSliceForward()
{    
    SetSlice(1);   
}

void CrossViewVtkInteractorStyle::MoveSliceBackward()
{
    SetSlice(-1);    
}

void CrossViewVtkInteractorStyle::OnKeyDown()
{
    std::string key = this->GetInteractor()->GetKeySym();
    if (key.compare("Up") == 0) {
        MoveSliceForward();
    } else if (key.compare("Down") == 0) {
        MoveSliceBackward();
    }

    //vtkInteractorStyleImage::OnKeyDown();
    vtkInteractorStyleTrackballCamera::OnKeyDown();

}

void CrossViewVtkInteractorStyle::OnMouseWheelForward()
{
    MoveSliceForward();
}

void CrossViewVtkInteractorStyle::OnMouseWheelBackward()
{
    MoveSliceBackward();
}


