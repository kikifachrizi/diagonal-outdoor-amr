#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "geometry_msgs/msg/twist_stamped.hpp"

class TwistToStampedNode : public rclcpp::Node
{
public:
  TwistToStampedNode() : Node("twist_to_stamped")
  {
    sub_ = this->create_subscription<geometry_msgs::msg::Twist>(
      "cmd_vel_in",
      rclcpp::SystemDefaultsQoS(),
      std::bind(&TwistToStampedNode::twistCallback, this, std::placeholders::_1)
    );

    pub_ = this->create_publisher<geometry_msgs::msg::TwistStamped>(
      "cmd_vel_out",
      rclcpp::SystemDefaultsQoS()
    );

    RCLCPP_INFO(this->get_logger(), "Twist → TwistStamped adapter started");
  }

private:
  void twistCallback(const geometry_msgs::msg::Twist::SharedPtr msg)
  {
    geometry_msgs::msg::TwistStamped stamped;
    stamped.header.stamp = this->now();
    stamped.header.frame_id = "base_link";  // aman, controller gak peduli
    stamped.twist = *msg;

    pub_->publish(stamped);
  }

  rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr sub_;
  rclcpp::Publisher<geometry_msgs::msg::TwistStamped>::SharedPtr pub_;
};

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<TwistToStampedNode>());
  rclcpp::shutdown();
  return 0;
}
