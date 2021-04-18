#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <time.h>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

#include "Eigen/Dense"

#include <llamba/base/multiply_parallel.hpp>
#include <llamba/generators/single_generator.hpp>

#define DATA_TYPE double
#define DATA_SIZE 400

using namespace std::chrono_literals;

class MinimalPublisher : public rclcpp::Node
{
  public:
    MinimalPublisher()
    : Node("minimal_publisher")
    {

        a = Eigen::MatrixXd::Random(DATA_SIZE, DATA_SIZE);
        b = Eigen::MatrixXd::Random(DATA_SIZE, DATA_SIZE);

        matrix_a = llamba::single_generator::generate_input<double>(DATA_SIZE);
        matrix_b = llamba::single_generator::generate_input<double>(DATA_SIZE);
        result   = llamba::single_generator::generate_input_zero<double>(DATA_SIZE);

        settings_ = llamba::settings(4_THREADS, PARALLELIZATION_STRATEGY::PTHREADS);
    
        publisher_ = this->create_publisher<std_msgs::msg::String>("topic", 10);
        timer_ = this->create_wall_timer(500ms, std::bind(&MinimalPublisher::timer_callback, this));
    }

  private:
    void timer_callback()
    {
      auto message = std_msgs::msg::String();
      message.data = std::to_string(clock());
      Eigen::MatrixXd c = a * b;
      //llamba::base::multiply_parallel<double> multiply = llamba::base::multiply_parallel<double>(matrix_a, matrix_b, result, settings_);
      publisher_->publish(message);
    }
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;

    std::vector<DATA_TYPE> matrix_a;
    std::vector<DATA_TYPE> matrix_b;
    std::vector<DATA_TYPE> result;

    llamba::settings settings_;

    Eigen::MatrixXd a;
    Eigen::MatrixXd b;
};

class MinimalSubscriber : public rclcpp::Node
{
  public:
    MinimalSubscriber()
    : Node("minimal_subscriber"), count(1)
    {
      subscription_ = this->create_subscription<std_msgs::msg::String>(
      "topic", 10, std::bind(&MinimalSubscriber::topic_callback, this, std::placeholders::_1));
    }

    virtual ~MinimalSubscriber()
    {
        std::cout << "Execution times after " << times.size() << " iterations (average): " << average(times) << "ms" << std::endl; 
        std::cout << "Execution times after " << times.size() << " iterations (standard deviation): " << standard_deviation(times) << "ms" << std::endl; 
    }

  private:

    double average(const std::vector<double>& values)
    {
        double average = 0.0;

        for(int i = 0; i < values.size(); i++)
        {
            average += values[i];
        }

        return average / values.size();
    }

    double standard_deviation(const std::vector<double>& values)
    {
        double average_ = average(values);

        double standard_deviation = 0.0;

        for(int i = 0; i < values.size(); i++)
        {
            standard_deviation += pow(average_ - values[i], 2);
        }

        standard_deviation /= values.size();

        return sqrt(standard_deviation);
    }

    void topic_callback(const std_msgs::msg::String::SharedPtr msg)
    {
      double time = (double)(clock() - atof(msg->data.c_str())) / CLOCKS_PER_SEC * 1000;
      times.push_back(time);
      RCLCPP_INFO(this->get_logger(), "I heard: '%d'", count++);
    }

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
    std::vector<double> times;
    size_t count;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    rclcpp::executors::MultiThreadedExecutor multithreaded_executor;
    auto pub_node = std::make_shared<MinimalPublisher>();
    auto sub_node = std::make_shared<MinimalSubscriber>();

    multithreaded_executor.add_node(pub_node);
    multithreaded_executor.add_node(sub_node);

    multithreaded_executor.spin();

    return 0;
}