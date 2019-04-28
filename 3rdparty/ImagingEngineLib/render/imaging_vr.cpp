#include "imaging_vr.h"
#include "renderer_raycasting_cpu.h"
#include "vector3.h"

using namespace DW::Render;

VRImaging::VRImaging()
{
	show_buffer_ = new ShowBuffer();
	show_buffer_->InitBufferData(512, 512, 32);
}

// ��ȡԭʼ��άͼ�����ݳ�
VolData* VRImaging::GetData()
{
	return volume_data_;
}
// ����ԭʼ��άͼ�����ݳ�
void VRImaging::SetData(VolData* data)
{
	volume_data_ = data;
}
// ��ȡ�����ʾͼ��
ShowBuffer* VRImaging::GetShowBuffer()
{
	if (renderer_){
		show_buffer_ = renderer_->GetShowBuffer();
		//if (vtk_image_data){
		//	//ԭ���ķ����������Ա����ɵ�ͼ��
		//	int width = vtk_image_data->GetDimensions()[0];
		//	int height = vtk_image_data->GetDimensions()[1];
	
		//	// ��8λת����32λ
		//	int slice = -1;

		//	UNITDATA3D* pdata = reinterpret_cast<UNITDATA3D*>( vtk_image_data->GetScalarPointer() );
		//	int number_of_components = vtk_image_data->GetNumberOfScalarComponents();
		//	
		//	show_buffer_->SetBufferData(pdata, width, height, number_of_components * 8);
		//}
	}
	return show_buffer_;
}
// ��Ⱦͼ�񣬻��浽show_buffer_
void VRImaging::Render()
{
	if (renderer_){
		renderer_->Render();
	}
}
// ����
void VRImaging::Zoom(int scale)
{

}
// �ƶ�
void VRImaging::Move(int distance) 
{

}
// ƽ������ת
void VRImaging::Rotate(float angle)
{

}
// ������������һ���Ƕ�
void VRImaging::Rotate3D(Vector3d axis, float angle) 
{

}
// ����λ
void VRImaging::WindowWidthLevel(int width, int level) 
{

}

void VRImaging::SetRenderer(IThreedRenderer* renderer)
{
	renderer_ = renderer;
}