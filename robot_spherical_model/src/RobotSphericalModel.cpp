/*
 * RobotSphericalModel.cpp
 *
 *  Created on: Aug 15, 2013
 *      Author: jfelip
 */

#include "robot_spherical_model/RobotSphericalModel.h"

RobotSphericalModel::RobotSphericalModel() {
	base_frame_="";
	modelFrames_.clear();
	modelSize_.clear();
	model_.clear();
	modelFrameTransforms_.clear();
}

void RobotSphericalModel::setModelSize(std::vector< double> &modelSize){
	modelSize_.clear();
	for (unsigned int i=0; i<modelSize.size(); i++)
		modelSize_.push_back(modelSize[i]);
	if (model_.size()-1 != modelSize_.size())
		model_.resize(modelSize_.size()-1);
}

void RobotSphericalModel::setModelFrames(std::vector< std::string> &modelFrames){
	modelFrames_.clear();
	for (unsigned int i=0; i<modelFrames.size(); i++)
		modelFrames_.push_back(modelFrames[i]);
	if (model_.size()-1 != modelFrames_.size())
		model_.resize(modelFrames_.size()-1);
}



double RobotSphericalModel::getDistanceToModel(double pt[3]){
	// Calculate the closest point in the bisphere axis
	double lambda;
	double distance = 99999999;
	double min_distance = 99999999;
	double offset = 0;
	double c2_1[3], c1_p[3], p_c[3];
	for (uint i = 0; i < model_.size(); i++)
	{
		if (model_[i].t == 1)
		{
		  p_c[0] = pt[0] - model_[i].C1.point.x;
		  p_c[1] = pt[1] - model_[i].C1.point.y;
		  p_c[2] = pt[2] - model_[i].C1.point.z;

		  distance = sqrt((p_c[0] * p_c[0]) + (p_c[1] * p_c[1]) + (p_c[2] * p_c[2]));
		  offset = model_[i].R1;
		  distance = distance - offset;
		  if (distance <= min_distance)
		  {
			  min_distance=distance;
		  }
		}
		else if (model_[i].t == 2)
		{
		  c2_1[0] = model_[i].C2.point.x - model_[i].C1.point.x;
		  c2_1[1] = model_[i].C2.point.y - model_[i].C1.point.y;
		  c2_1[2] = model_[i].C2.point.z - model_[i].C1.point.z;
		  c1_p[0] = model_[i].C1.point.x - pt[0];
		  c1_p[1] = model_[i].C1.point.y - pt[1];
		  c1_p[2] = model_[i].C1.point.z - pt[2];
		  lambda = -((c2_1[0] * c1_p[0]) + (c2_1[1] * c1_p[1]) + (c2_1[2] * c1_p[2])) / (model_[i].norm * model_[i].norm);
		  if (lambda > 1)
			lambda = 1.0;
		  else if (lambda < 0)
			lambda = 0.0;
		  p_c[0] = pt[0] - (lambda*model_[i].C1.point.x + (1-lambda)*(model_[i].C2.point.x));
		  p_c[1] = pt[1] - (lambda*model_[i].C1.point.y + (1-lambda)*(model_[i].C2.point.y));
		  p_c[2] = pt[2] - (lambda*model_[i].C1.point.z + (1-lambda)*(model_[i].C2.point.z));
		  offset = (lambda*model_[i].R1 + (1-lambda)*model_[i].R2);
		  distance = sqrt( (p_c[0] * p_c[0]) + (p_c[1] * p_c[1]) + (p_c[2] * p_c[2]) );
		  distance = distance - offset;
		  if (distance <= min_distance)
		  {
			  min_distance=distance;
		  }
		}
	}
	return (min_distance);
}


bool RobotSphericalModel::isModelPoint(double pt[3]){
	// Calculate the closest point in the bisphere axis
	double lambda;
	double distance = 99999999;
	double offset = 0;
	double c2_1[3], c1_p[3], p_c[3];
	for (uint i = 0; i < model_.size(); i++)
	{
		if (model_[i].t == 1)
		{
		  p_c[0] = (double)pt[0] - model_[i].C1.point.x;
		  p_c[1] = (double)pt[1] - model_[i].C1.point.y;
		  p_c[2] = (double)pt[2] - model_[i].C1.point.z;

		  distance = (p_c[0] * p_c[0]) + (p_c[1] * p_c[1]) + (p_c[2] * p_c[2]);
		  offset = (model_[i].R1 * model_[i].R1);
		  if (distance <= (offset))
		  {
			return (true);
		  }
		}
		else if (model_[i].t == 2)
		{
		  c2_1[0] = model_[i].C2.point.x - model_[i].C1.point.x;
		  c2_1[1] = model_[i].C2.point.y - model_[i].C1.point.y;
		  c2_1[2] = model_[i].C2.point.z - model_[i].C1.point.z;
		  c1_p[0] = model_[i].C1.point.x - pt[0];
		  c1_p[1] = model_[i].C1.point.y - pt[1];
		  c1_p[2] = model_[i].C1.point.z - pt[2];
		  lambda = -((c2_1[0] * c1_p[0]) + (c2_1[1] * c1_p[1]) + (c2_1[2] * c1_p[2])) / (model_[i].norm * model_[i].norm);
		  if (lambda > 1)
			lambda = 1.0;
		  else if (lambda < 0)
			lambda = 0.0;
		  p_c[0] = pt[0] - (lambda*model_[i].C1.point.x + (1-lambda)*(model_[i].C2.point.x));
		  p_c[1] = pt[1] - (lambda*model_[i].C1.point.y + (1-lambda)*(model_[i].C2.point.y));
		  p_c[2] = pt[2] - (lambda*model_[i].C1.point.z + (1-lambda)*(model_[i].C2.point.z));
		  offset = (lambda*model_[i].R1 + (1-lambda)*model_[i].R2);
		  distance = sqrt( (p_c[0] * p_c[0]) + (p_c[1] * p_c[1]) + (p_c[2] * p_c[2]) );
		  if (distance <= offset)
		  {
			return (true);
		  }
		}
	}
	return (false);
}


#ifdef USE_ROS
void RobotSphericalModel::updateModelROS(tf::TransformListener * tf_listener)
{
    ros::Time t;
    t = ros::Time(0);
    for (uint i=0; i<model_.size(); i++){
        model_[i].t = 2;
        model_[i].C1.point.x = 0.0;
        model_[i].C1.point.y = 0.0;
        model_[i].C1.point.z = 0.0;
        model_[i].C1.header.frame_id = modelFrames_[i];
        model_[i].C1.header.stamp = t;
        model_[i].R1 = modelSize_[i];

        model_[i].C2.point.x = 0.0;
        model_[i].C2.point.y = 0.0;
        model_[i].C2.point.z = 0.0;
        model_[i].C2.header.stamp = t;
        if (modelFrames_[i+1] != modelFrames_[0]){
            model_[i].C2.header.frame_id = modelFrames_[i+1];
            model_[i].R2 = modelSize_[i+1];
        }
        else{
            model_[i].C2.header.frame_id = modelFrames_[i];
            model_[i].R2 = modelSize_[i];
        }

        try{
            tf_listener->transformPoint(base_frame_, model_[i].C1, model_[i].C1);
            tf_listener->transformPoint(base_frame_, model_[i].C2, model_[i].C2);
        }
        catch (tf::TransformException &ex){
            ROS_ERROR("%s",ex.what());
        }

        model_[i].norm = sqrt( (model_[i].C2.point.x-model_[i].C1.point.x) * (model_[i].C2.point.x-model_[i].C1.point.x) +
                               (model_[i].C2.point.y-model_[i].C1.point.y) * (model_[i].C2.point.y-model_[i].C1.point.y) +
                               (model_[i].C2.point.z-model_[i].C1.point.z) * (model_[i].C2.point.z-model_[i].C1.point.z) );
    }
}

void RobotSphericalModel::getModelMarkers(std::string name ,std_msgs::ColorRGBA &color, visualization_msgs::MarkerArray &markers){
	int id = 0;
	for (uint i=0;i<model_.size(); i++){
		//Initial sphere
		visualization_msgs::Marker marker;
		marker.header.frame_id = base_frame_;
		marker.header.stamp = ros::Time();
		marker.ns = name;
		marker.id = id;
		id++;
		marker.type = visualization_msgs::Marker::SPHERE;
		marker.action = visualization_msgs::Marker::ADD;
		marker.pose.position.x = model_[i].C1.point.x;
		marker.pose.position.y = model_[i].C1.point.y;
		marker.pose.position.z = model_[i].C1.point.z;
		marker.pose.orientation.x = 0.0;
		marker.pose.orientation.y = 0.0;
		marker.pose.orientation.z = 0.0;
		marker.pose.orientation.w = 1.0;
		marker.scale.x = model_[i].R1;
		marker.scale.y = model_[i].R1;
		marker.scale.z = model_[i].R1;
		marker.color.r = color.r;
		marker.color.g = color.g;
		marker.color.b = color.b;
		marker.color.a = color.a;
		markers.markers.push_back(marker);

		//Intermediate spheres
		double lambda = 0;
		double step = 0.1;
		while (lambda<=1.0){
			visualization_msgs::Marker mk;
			mk.header.frame_id = base_frame_;
			mk.header.stamp = ros::Time();
			mk.ns = name;
			mk.id = id;
			id++;
			mk.type = visualization_msgs::Marker::SPHERE;
			mk.action = visualization_msgs::Marker::ADD;
			mk.pose.position.x = model_[i].C1.point.x + lambda * (model_[i].C2.point.x - model_[i].C1.point.x);
			mk.pose.position.y = model_[i].C1.point.y + lambda * (model_[i].C2.point.y - model_[i].C1.point.y);
			mk.pose.position.z = model_[i].C1.point.z + lambda * (model_[i].C2.point.z - model_[i].C1.point.z);
			mk.pose.orientation.x = 0.0;
			mk.pose.orientation.y = 0.0;
			mk.pose.orientation.z = 0.0;
			mk.pose.orientation.w = 1.0;
			mk.scale.x = model_[i].R1*(1-lambda) + model_[i].R2*lambda;
			mk.scale.y = model_[i].R1*(1-lambda) + model_[i].R2*lambda;
			mk.scale.z = model_[i].R1*(1-lambda) + model_[i].R2*lambda;
			mk.color.r = color.r;
			mk.color.g = color.g;
			mk.color.b = color.b;
			mk.color.a = color.a;
			markers.markers.push_back(mk);
			lambda += step;
		}

		//Final sphere
		visualization_msgs::Marker marker2;
		marker2.header.frame_id = base_frame_;
		marker2.header.stamp = ros::Time();
		marker2.ns = name;
		marker2.id = id;
		id++;
		marker2.type = visualization_msgs::Marker::SPHERE;
		marker2.action = visualization_msgs::Marker::ADD;
		marker2.pose.position.x = model_[i].C2.point.x;
		marker2.pose.position.y = model_[i].C2.point.y;
		marker2.pose.position.z = model_[i].C2.point.z;
		marker2.pose.orientation.x = 0.0;
		marker2.pose.orientation.y = 0.0;
		marker2.pose.orientation.z = 0.0;
		marker2.pose.orientation.w = 1.0;
		marker2.scale.x = model_[i].R2;
		marker2.scale.y = model_[i].R2;
		marker2.scale.z = model_[i].R2;
		marker2.color.r = color.r;
		marker2.color.g = color.g;
		marker2.color.b = color.b;
		marker2.color.a = color.a;
		markers.markers.push_back(marker2);
	}
}
#else
void RobotSphericalModel::updateModel(std::vector< RobotSphericalModel::CTransform > transforms )
{
    if ( transforms.size() != model_.size()+1 )
        return;

    if ( modelSize_.size() != model_.size()+1 )
        return;

    if ( modelFrames_.size() != model_.size()+1 )
        return;

    for (uint i=0; i<model_.size(); i++)
    {
        model_[i].t = 2;
        model_[i].C1.header.frame_id = modelFrames_[i];
        model_[i].R1 = modelSize_[i];
        model_[i].C1.point.x = transforms[i].position[0];
        model_[i].C1.point.y = transforms[i].position[1];
        model_[i].C1.point.z = transforms[i].position[2];

        model_[i].C2.header.frame_id = modelFrames_[i+1];
        model_[i].R2 = modelSize_[i+1];
        model_[i].C2.point.x = transforms[i+1].position[0];
        model_[i].C2.point.y = transforms[i+1].position[1];
        model_[i].C2.point.z = transforms[i+1].position[2];

        model_[i].norm = sqrt( (model_[i].C2.point.x-model_[i].C1.point.x) * (model_[i].C2.point.x-model_[i].C1.point.x) +
                               (model_[i].C2.point.y-model_[i].C1.point.y) * (model_[i].C2.point.y-model_[i].C1.point.y) +
                               (model_[i].C2.point.z-model_[i].C1.point.z) * (model_[i].C2.point.z-model_[i].C1.point.z) );
    }

}
#endif
