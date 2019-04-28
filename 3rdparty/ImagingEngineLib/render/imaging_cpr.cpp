#include "imaging_cpr.h"
#include "renderer_cpr.h"
#include "vector3.h"

using namespace DW::Render;

CPRImaging::CPRImaging()
{
	show_buffer_ = new ShowBuffer();
	show_buffer_->InitBufferData(512, 512, 32);
}

// ��ȡԭʼ��άͼ�����ݳ�
VolData* CPRImaging::GetData()
{
	return volume_data_;
}
// ����ԭʼ��άͼ�����ݳ�
void CPRImaging::SetData(VolData* data)
{
	volume_data_ = data;
}
// ��ȡ�����ʾͼ��
ShowBuffer* CPRImaging::GetShowBuffer()
{
	if (renderer_){
		show_buffer_ = renderer_->GetShowBuffer();
	}
	return show_buffer_;
}
// ��Ⱦͼ�񣬻��浽show_buffer_
void CPRImaging::Render()
{
	if (renderer_){
		renderer_->Render();
	}
}

void CPRImaging::SetRenderer(IThreedRenderer* renderer)
{
	renderer_ = renderer;
}