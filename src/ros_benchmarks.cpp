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
#define DATA_TYPE_INT int
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

    void multiply_serial(const std::vector<DATA_TYPE>& matrix_a, const std::vector<DATA_TYPE>& matrix_b, std::vector<DATA_TYPE>& result_serial)
    {
      for(int i = 0; i < DATA_SIZE; i++)
      {
        for(int j = 0; j < DATA_SIZE; j++)
        {
          for(int k = 0; k < DATA_SIZE; k++)
          {
            result_serial[i * DATA_SIZE + j] +=  matrix_a[i * DATA_SIZE + k] * matrix_b[k * DATA_SIZE + j];
          }
        }
      }
    }

    void sum_serial(const std::vector<DATA_TYPE>& matrix_a, const std::vector<DATA_TYPE>& matrix_b, std::vector<DATA_TYPE>& result_serial)
    {
      for(int i = 0; i < DATA_SIZE; i++)
      {
        for(int j = 0; j < DATA_SIZE; j++)
        {
          result_serial[i * DATA_SIZE + j] =  matrix_a[i * DATA_SIZE + j] + matrix_b[i * DATA_SIZE + j];
        }
      }
    }

    void abstract_serial(const std::vector<DATA_TYPE>& matrix_a, const std::vector<DATA_TYPE>& matrix_b, std::vector<DATA_TYPE>& result_serial)
    {
      for(int i = 0; i < DATA_SIZE; i++)
      {
        for(int j = 0; j < DATA_SIZE; j++)
        {
          result_serial[i * DATA_SIZE + j] =  matrix_a[i * DATA_SIZE + j] - matrix_b[i * DATA_SIZE + j];
        }
      }
    }

    void getCofactor(std::vector<DATA_TYPE>& A, std::vector<DATA_TYPE>& temp, int p, int q, int n)
    {
        int i = 0, j = 0;
        for (int row = 0; row < n; row++)
        {
            for (int col = 0; col < n; col++)
            {
                if (row != p && col != q)
                {
                    temp[i * DATA_SIZE + j++] = A[row * DATA_SIZE + col];
      
                    if (j == n - 1)
                    {
                        j = 0;
                        i++;
                    }
                }
            }
        }
    }

    int determinant(std::vector<DATA_TYPE>& A, int n)
    {
        int D = 0; 

        if (n == 1)
            return A[0];
      
        std::vector<DATA_TYPE> temp(DATA_SIZE * DATA_SIZE); 
      
        int sign = 1; 
      
        for (int f = 0; f < n; f++)
        {
            getCofactor(A, temp, 0, f, n);
            D += sign * A[0 * DATA_SIZE + f] * determinant(temp, n - 1);
      
            sign = -sign;
        }
      
        return D;
    }
  
    void adjoint(std::vector<DATA_TYPE> A,std::vector<DATA_TYPE> adj)
    {
        if (DATA_SIZE == 1)
        {
            adj[0] = 1;
            return;
        }
      
        int sign = 1; 
        std::vector<DATA_TYPE> temp(DATA_SIZE * DATA_SIZE);
      
        for (int i=0; i<DATA_SIZE; i++)
        {
            for (int j=0; j<DATA_SIZE; j++)
            {
                getCofactor(A, temp, i, j, DATA_SIZE);

                sign = ((i+j)%2==0)? 1: -1;
      
                adj[j * DATA_SIZE + i] = (sign)*(determinant(temp, DATA_SIZE-1));
            }
        }
    }
  
    bool inverse_serial(std::vector<DATA_TYPE> A, std::vector<DATA_TYPE>& inverse)
    {
        int det = determinant(A, DATA_SIZE);

        if (det == 0)
        {
            std::cout << "Singular matrix, can't find its inverse";
            return false;
        }
      
        
        std::vector<DATA_TYPE> adj(DATA_SIZE * DATA_SIZE);
        adjoint(A, adj);
      
        for (int i=0; i<DATA_SIZE; i++)
            for (int j=0; j<DATA_SIZE; j++)
                inverse[i * DATA_SIZE + j] = adj[i * DATA_SIZE + j]/float(det);
      
        return true;
    }

    void blue_channel_serial(std::vector<DATA_TYPE_INT>& matrix_a, const std::vector<DATA_TYPE_INT>& matrix_b, std::vector<DATA_TYPE_INT>& result_serial)
    {
      for(int i = 0; i < DATA_SIZE; i++)
      {
        for(int j = 0; j < DATA_SIZE; j++)
        {
          result_serial[i * DATA_SIZE + j] =  (matrix_a[i * DATA_SIZE + j] >> 16) & 0x000000FF;
        }
      }
    }

    void complex_serial(const std::vector<DATA_TYPE>& matrix_a, const std::vector<DATA_TYPE>& matrix_b, std::vector<DATA_TYPE>& result_serial)
    {
      for(int i = 0; i < DATA_SIZE; i++)
      {
        for(int j = 0; j < DATA_SIZE; j++)
        {
            result_serial[i * DATA_SIZE + j] =  matrix_a[i * DATA_SIZE + j] * matrix_b[i * DATA_SIZE + j] + pow(matrix_a[i * DATA_SIZE + j], 2);
        }
      }
    }

    void grayscale_channel_serial(const std::vector<DATA_TYPE_INT>& matrix_a, const std::vector<DATA_TYPE_INT>& matrix_b, std::vector<DATA_TYPE_INT>& result_serial)
    {
      for(int i = 0; i < DATA_SIZE; i++)
      {
        for(int j = 0; j < DATA_SIZE; j++)
        {
          result_serial[i * DATA_SIZE + j] = (((matrix_a[i * DATA_SIZE + j])&0x000000FF) + ((matrix_a[i * DATA_SIZE + j] >> 8)&0x000000FF) + ((matrix_a[i * DATA_SIZE + j] >> 16)&0x000000FF)) / 3;
        }
      }
    }

    void green_channel_serial(const std::vector<DATA_TYPE_INT>& matrix_a, const std::vector<DATA_TYPE_INT>& matrix_b, std::vector<DATA_TYPE_INT>& result_serial)
    {
      for(int i = 0; i < DATA_SIZE; i++)
      {
        for(int j = 0; j < DATA_SIZE; j++)
        {
          result_serial[i * DATA_SIZE + j] =  (matrix_a[i * DATA_SIZE + j] >> 8) & 0x000000FF;
        }
      }
    }

    void red_channel_serial(const std::vector<DATA_TYPE_INT>& matrix_a, const std::vector<DATA_TYPE_INT>& matrix_b, std::vector<DATA_TYPE_INT>& result_serial)
    {
      for(int i = 0; i < DATA_SIZE; i++)
      {
        for(int j = 0; j < DATA_SIZE; j++)
        {
          result_serial[i * DATA_SIZE + j] =  matrix_a[i * DATA_SIZE + j] & 0x000000FF;
        }
      }
    }

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