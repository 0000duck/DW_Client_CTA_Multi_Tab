/*=========================================================================

  Program:   ImagingEngine
  Module:    curve_line.h
  author: 	 zhangjian
  Brief:	 ��ά�����࣬ͨ��cardinal������ֵ���ɲ�����

=========================================================================*/
#pragma once

#include "data/data_definition.h"
#include "data/vector2.h"
#include "data/vector3.h"

using namespace std;

namespace DW {
	namespace IMAGE {

		class VolCurve
		{
		public:
			VolCurve();
			~VolCurve();

			/// ��ȡ���߱��
			string GetCurveID() {return curve_id_;}
			/// ���һ����ά���Ƶ�
			virtual void AddControlPoint(Point3f);
			virtual void AddControlPoint(float x, float y, float z);
			/// �Ƴ�һ����ά���Ƶ�
			virtual void RemoveControlPoint(int);
			/// ����ָ��λ�õĿ��Ƶ�����
			virtual void GetControlPoint(int pos, float& x, float& y, float& z);
			/// ����ָ��λ�õĲ���������
			virtual void GetSamplePoint(int pos, float& x, float& y, float& z);
			/// ���в���
			virtual void Update();
			/// ���Ƶ���Ŀ
			virtual int GetNumberOfControlPoint()
			{
				return control_points_.size();
			}
			/// ��������Ŀ
			virtual int GetNumberOfSamplePoint()
			{
				return sample_points_.size();
			}
			/// ���ò������
			virtual void SetSamplingInterval(float interval)
			{
				sampling_interval_ = interval;
			}
			/// ���ز������
			virtual float GetSamplingInterval()
			{
				return sampling_interval_;
			}
			/// ��������֮����Ƴ��ȣ���λ�����أ�
			virtual float CalculateDistance(int from_index, int to_index);
			/// ��ȡ���߷���
			virtual void GetCurveDirection(float& x, float& y, float& z);
			/// ��ȡ����������߷�������
			void GetTangentVector(int index, Vector3f& tangent);
			/// ��ȡ������ֲ�ƽ��ķ�������
			void GetLocalPlaneVector(int index, Vector3f& x, Vector3f& y);

		protected:
			/// �ж��Ƿ���ͬһ����
			bool IsSame3DPoint(Point3f ControlPoint);
			/// ʵ�����ɲ�����
			void GenerateCurve();
			/// ��������������/����/����������
			void ComputeSampleVectors();
			/// ����һ����������
			void ComputeTangentVector(Point3f last_pnt, Point3f next_pnt, Vector3f& tangent);
			/// ����һ����������
			void ComputeNormalVector(Vector3f last_tangent, Vector3f next_tangent, Vector3f& normal);
			/// ���ݹ����Լ���һ����������
			void ComputeConsistentNormalVector(Vector3f tangent, Vector3f last_normal, Vector3f& normal);

		protected:
			/// ���߱��
			string curve_id_;
			/// ��ά���ݳ����Ƶ��б�
			vector<Point3f> control_points_;
			/// ��ά���ݳ��������б�
			vector<Point3f> sample_points_;
			/// �������
			float sampling_interval_;
			/// ÿ�����������������
			vector<Vector3f> tangent_vectors_;
			/// ÿ��������ķ�������
			vector<Vector3f> normal_vectors_;
			/// ÿ��������ĸ���������
			vector<Vector3f> binormal_vectors_;
			/// ������֮�������
			bool using_consistent_normal_;
		};

	}
}