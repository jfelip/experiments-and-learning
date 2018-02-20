/*
 * RobotSphericalModel.h
 *
 *  Created on: Aug 15, 2013
 *      Author: jfelip
 */

#ifndef ROBOTSPHERICALMODEL_H_
#define ROBOTSPHERICALMODEL_H_

#include <vector>
#include <string>
#include <cmath>

#ifdef USE_ROS
	#include <tf/tf.h>
	#include <tf/transform_listener.h>
	#include <std_msgs/ColorRGBA.h>
	#include <visualization_msgs/Marker.h>
	#include <visualization_msgs/MarkerArray.h>
#endif

class RobotSphericalModel {
	private:

#ifndef USE_ROS
    //Proxy classes to replace their ROS counterparts
    struct Header
    {
        std::string frame_id;
        time_t stamp;
    };

    struct Point
    {
        double x;
        double y;
        double z;
    };

    struct PointStamped
    {
        Header header;
        Point point;
    };
#endif

	typedef struct model
	{
#ifdef USE_ROS
        geometry_msgs::PointStamped C1;
        geometry_msgs::PointStamped C2;
#else
        PointStamped C1;
        PointStamped C2;
#endif
        double R1;
        double R2;
        double norm;
        int t;
	} model_link;

    class CTransform
    {
    public:
        double position[3];
        double orientation[4];
    };

	std::string base_frame_;
	std::vector< std::string> modelFrames_;
	std::vector< double> modelSize_;
	std::vector< model_link> model_;

#ifdef USE_ROS
	std::vector< tf::StampedTransform> modelFrameTransforms_;
#else
    std::vector< CTransform > modelFrameTransforms_;
#endif

	public:
	RobotSphericalModel();
	~RobotSphericalModel() = default;

#ifdef USE_ROS
/** Update the position of the spherical model.
	* @param tf_listener A pointer to a valid tf::TransformListener that will be used to
	* compute transformations between the different model frames
	*/
	void updateModel(tf::TransformListener * tf_listener_);

	/** Obtain a RVIZ marker array representing the model.
	* @param name Name for the namespace of the markers
	* @param color
	*/
	void getModelMarkers(std::string name ,std_msgs::ColorRGBA &color, visualization_msgs::MarkerArray &markers);
#else
    /** Update the positions of the spherical models.
	* @param transforms Vector of the transforms where each frame of the model is currently located.
	*/
    void updateModel(std::vector< RobotSphericalModel::CTransform > transforms );
#endif


	/** Determine whether the given point is inside the model or not.
	* @param pt 3D point to be considered. It has to be in the base_frame.
	* @return true if the point is inside the model, false otherwise
	*/
	bool isModelPoint(double pt[3]);

	/** Determine whether the given point is inside the model or not.
	* @param pt 3D point to be considered. It has to be in the base_frame.
	* @return Shortest distance from the given point to the model. If the
	* point is inside the model the distance will be negative.
	*/
	double getDistanceToModel(double pt[3]);

	/** set Base frame for the spherical model.
	* @param base_frame
	*/
	void setBaseFrame(std::string &base_frame){base_frame_ = base_frame;}
	/** set Frame names that correspond to the model joints, the names have to be ordered from base to end.
	* @param arm
	*/
	void setModelFrames(std::vector< std::string> &modelFrames);
	/** set Radius of the s-tope spheres that will compose the model geometry for each joint.
	* @param armSize
	*/
	void setModelSize(std::vector< double> &modelSize);


};

#endif /* ROBOTSPHERICALMODEL_H_ */
