#ifndef __ODOMETRY_HPP__
#define __ODOMETRY_HPP__

#include <iostream>
#include <chrono>
#include <thread>
#include <pthread.h>
#include <mutex>
#include <utility>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>

#define THREAD_POOL 4
#define THREAD_NUMBER 4
#define WIDTH 30

namespace llamba
{

namespace algorithm
{

typedef std::milli TIME_MEASUREMENT;

std::mutex mutexes[THREAD_NUMBER];

typedef struct
{
  double v[3];
} pf_vector_t;

typedef struct
{
  // Pose represented by this sample
  pf_vector_t pose;

  // Weight for this pose
  double weight;
} pf_sample_t;

// Info for a node in the tree
typedef struct pf_kdtree_node
{
  // Depth in the tree
  int leaf, depth;

  // Pivot dimension and value
  int pivot_dim;
  double pivot_value;

  // The key for this node
  int key[3];

  // The value for this node
  double value;

  // The cluster label (leaf nodes)
  int cluster;

  // Child nodes
  struct pf_kdtree_node * children[2];
} pf_kdtree_node_t;

// A kd tree
typedef struct
{
  // Cell size
  double size[3];

  // The root node of the tree
  pf_kdtree_node_t * root;

  // The number of nodes in the tree
  int node_count, node_max_count;
  pf_kdtree_node_t * nodes;

  // The number of leaf nodes in the tree
  int leaf_count;
} pf_kdtree_t;

// The basic matrix
typedef struct
{
  double m[3][3];
} pf_matrix_t;

typedef struct
{
  // Number of samples
  int count;

  // Total weight of samples in this cluster
  double weight;

  // Cluster statistics
  pf_vector_t mean;
  pf_matrix_t cov;

  // Workspace
  double m[4], c[2][2];
} pf_cluster_t;

typedef struct _pf_sample_set_t
{
  // The samples
  int sample_count;
  pf_sample_t * samples;

  // A kdtree encoding the histogram
  pf_kdtree_t * kdtree;

  // Clusters
  int cluster_count, cluster_max_count;
  pf_cluster_t * clusters;

  // Filter statistics
  pf_vector_t mean;
  pf_matrix_t cov;
  int converged;
} pf_sample_set_t;

typedef pf_vector_t (* pf_init_model_fn_t) (void * init_data);

typedef struct _pf_t
{
  // This min and max number of samples
  int min_samples, max_samples;

  // Population size parameters
  double pop_err, pop_z;

  // The sample sets.  We keep two sets and use [current_set]
  // to identify the active set.
  int current_set;
  pf_sample_set_t sets[2];

  // Running averages, slow and fast, of likelihood
  double w_slow, w_fast;

  // Decay rates for running averages
  double alpha_slow, alpha_fast;

  // Function used to draw random pose samples
  pf_init_model_fn_t random_pose_fn;
  void * random_pose_data;

  double dist_threshold;  // distance threshold in each axis over which the pf is considered to not be converged
  int converged;
} pf_t;

pf_t * pf = new pf_t(); 
pf_vector_t  pose; 
pf_vector_t  delta;

struct iterators 
{

  iterators(
    int begin, 
    int end, 
    double & alpha1_, 
    double & alpha2_, 
    double & alpha3_, 
    double & alpha4_, 
    pf_sample_set_t *& set, 
    double & delta_rot1, 
    double & delta_rot2,
    double & delta_rot1_noise, 
    double & delta_rot2_noise,
    double & delta_trans,
    double & delta_trans_hat,
    double & delta_rot1_hat,
    double & delta_rot2_hat) : 
      begin(begin),
      end(end),
      alpha1_(alpha1_),
      alpha2_(alpha2_),
      alpha3_(alpha3_),
      alpha4_(alpha4_),
      set(set),
      delta_rot1(delta_rot1),
      delta_rot2(delta_rot2),
      delta_rot1_noise(delta_rot1_noise),
      delta_rot2_noise(delta_rot2_noise),
      delta_trans(delta_trans),
      delta_trans_hat(delta_trans_hat),
      delta_rot1_hat(delta_rot1_hat),
      delta_rot2_hat(delta_rot2_hat) { }

  int begin;
  int end;
  double & alpha1_; 
  double & alpha2_;
  double & alpha3_; 
  double & alpha4_;
  pf_sample_set_t *& set;
  
  double & delta_rot1;
  double & delta_rot2;
  double & delta_rot1_noise;
  double & delta_rot2_noise;
  double & delta_trans;
  double & delta_trans_hat;
  double & delta_rot1_hat;
  double & delta_rot2_hat;
};

struct odometry_struct
{
  void (*fn) (iterators*);
  int thread_number;
  bool is_finished;
  bool is_processing;
  
  odometry_struct(void (*fn) (iterators* iterators_), int thread_number) : fn(fn), thread_number(thread_number), is_finished(false), is_processing(false) {};
};

std::pair<odometry_struct, iterators*>* tasks[THREAD_POOL];

void * pool_thread_worker(void * arg__)
{
  int* arg = (int*) arg__;
  while(true)
  {
    if(tasks[*arg] != NULL)
    {
      auto p = tasks[*arg];
      if(!p->first.is_finished)
      {
        p->first.fn(p->second);
        p->first.is_finished = true;   
      }
    }
  }

  return NULL;
}

pf_vector_t pf_vector_sub(pf_vector_t a, pf_vector_t b)
{
  pf_vector_t c;

  c.v[0] = a.v[0] - b.v[0];
  c.v[1] = a.v[1] - b.v[1];
  c.v[2] = a.v[2] - b.v[2];

  return c;
}

class angleutils
{
public:
  /*
   * @brief Normalize angles
   * @brief z Angle to normalize
   * @return normalized angle
   */
  static double normalize(double z);

  /*
   * @brief Find minimum distance between 2 angles
   * @brief a Angle 1
   * @brief b Angle 2
   * @return normalized angle difference
   */
  static double angle_diff(double a, double b);
};

inline double angleutils::normalize(double z)
{
  return atan2(sin(z), cos(z));
}

inline double angleutils::angle_diff(double a, double b)
{
  a = atan2(sin(a), cos(a));
  b = atan2(sin(b), cos(b));
  double d1 = a - b;
  double d2 = 2 * M_PI - fabs(d1);
  if (d1 > 0) {
    d2 *= -1.0;
  }
  if (fabs(d1) < fabs(d2)) {
    return d1;
  } else {
    return d2;
  }
}

double drand48(void)
{
  return ((double)rand()) / RAND_MAX;
}

double pf_ran_gaussian(double sigma)
{
  double x1, x2, w, r;

  do {
    do {
      r = drand48();
    } while (r == 0.0);
    x1 = 2.0 * r - 1.0;
    do {
      r = drand48();
    } while (r == 0.0);
    x2 = 2.0 * r - 1.0;
    w = x1 * x1 + x2 * x2;
  } while (w > 1.0 || w == 0.0);

  return sigma * x2 * sqrt(-2.0 * log(w) / w);
}


unsigned long int odometryUpdate(pf_t * pf, const pf_vector_t & pose, const pf_vector_t & delta, int iterations)
{
  double alpha1_ = 0.5;
  double alpha2_ = 0.5;
  double alpha3_ = 0.5;
  double alpha4_ = 0.5;

  // Compute the new sample poses
  pf_sample_set_t * set;

  set = pf->sets + pf->current_set;
  pf_vector_t old_pose = pf_vector_sub(pose, delta);

  // Implement sample_motion_odometry (Prob Rob p 136)
  double delta_rot1, delta_trans, delta_rot2;
  double delta_rot1_hat, delta_trans_hat, delta_rot2_hat;
  double delta_rot1_noise, delta_rot2_noise;

  if (sqrt(
      delta.v[1] * delta.v[1] +
      delta.v[0] * delta.v[0]) < 0.01)
  {
    delta_rot1 = 0.0;
  } else {
    delta_rot1 = angleutils::angle_diff(
      atan2(delta.v[1], delta.v[0]),
      old_pose.v[2]);
  }
  delta_trans = sqrt(
    delta.v[0] * delta.v[0] +
    delta.v[1] * delta.v[1]);
  delta_rot2 = angleutils::angle_diff(delta.v[2], delta_rot1);

  delta_rot1_noise = std::min(
    fabs(angleutils::angle_diff(delta_rot1, 0.0)),
    fabs(angleutils::angle_diff(delta_rot1, M_PI)));
  delta_rot2_noise = std::min(
    fabs(angleutils::angle_diff(delta_rot2, 0.0)),
    fabs(angleutils::angle_diff(delta_rot2, M_PI)));


  set->sample_count = 2000;

  double pose1,pose2,pose3;

  auto before = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < iterations; i++) {
    pf_sample_t * sample = set->samples + i;

    // Sample pose differences
    delta_rot1_hat = angleutils::angle_diff(
      delta_rot1,
      pf_ran_gaussian(
        sqrt(
          alpha1_ * delta_rot1_noise * delta_rot1_noise +
          alpha2_ * delta_trans * delta_trans)));
    delta_trans_hat = delta_trans -
      pf_ran_gaussian(
      sqrt(
        alpha3_ * delta_trans * delta_trans +
        alpha4_ * delta_rot1_noise * delta_rot1_noise +
        alpha4_ * delta_rot2_noise * delta_rot2_noise));
    delta_rot2_hat = angleutils::angle_diff(
      delta_rot2,
      pf_ran_gaussian(
        sqrt(
          alpha1_ * delta_rot2_noise * delta_rot2_noise +
          alpha2_ * delta_trans * delta_trans)));

    
    // Apply sampled update to particle pose
    pose1 += delta_trans_hat *
      cos(pose2 + delta_rot1_hat);
    pose1 += delta_trans_hat *
      sin(pose2 + delta_rot1_hat);
    pose2 += delta_rot1_hat + delta_rot2_hat;
  }
  auto after = std::chrono::high_resolution_clock::now();

  return std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT> >(after - before).count();
}

unsigned long int odometryUpdateMultithreading(pf_t * pf, const pf_vector_t & pose, const pf_vector_t & delta, int iterations)
{
  double alpha1_ = 0.5;
  double alpha2_ = 0.5;
  double alpha3_ = 0.5;
  double alpha4_ = 0.5;

  // Compute the new sample poses
  pf_sample_set_t * set;

  set = pf->sets + pf->current_set;
  pf_vector_t old_pose = pf_vector_sub(pose, delta);

  // Implement sample_motion_odometry (Prob Rob p 136)
  double delta_rot1, delta_trans, delta_rot2;
  double delta_rot1_hat, delta_trans_hat, delta_rot2_hat;
  double delta_rot1_noise, delta_rot2_noise;

  if (sqrt(
      delta.v[1] * delta.v[1] +
      delta.v[0] * delta.v[0]) < 0.01)
  {
    delta_rot1 = 0.0;
  } else {
    delta_rot1 = angleutils::angle_diff(
      atan2(delta.v[1], delta.v[0]),
      old_pose.v[2]);
  }
  delta_trans = sqrt(
    delta.v[0] * delta.v[0] +
    delta.v[1] * delta.v[1]);
  delta_rot2 = angleutils::angle_diff(delta.v[2], delta_rot1);

  delta_rot1_noise = std::min(
    fabs(angleutils::angle_diff(delta_rot1, 0.0)),
    fabs(angleutils::angle_diff(delta_rot1, M_PI)));
  delta_rot2_noise = std::min(
    fabs(angleutils::angle_diff(delta_rot2, 0.0)),
    fabs(angleutils::angle_diff(delta_rot2, M_PI)));




  odometry_struct settings_ = odometry_struct([](iterators* iterators_) -> void {

    int begin =     iterators_->begin;

    double pose1 = 0.0, pose2 = 0.0;

    int end = iterators_->end;

    pf_sample_t * pff =  iterators_->set->samples;


    double delta_rot1 = iterators_->delta_rot1;

    double alpha1_ = iterators_->alpha1_;
    double alpha2_ = iterators_->alpha2_;
    double alpha3_ = iterators_->alpha3_;
    double alpha4_ = iterators_->alpha4_;

    double delta_rot1_noise = iterators_->delta_rot1_noise;
    double delta_trans = iterators_->delta_trans;

    double delta_rot2_noise = iterators_->delta_rot2_noise;
    double delta_rot2 = iterators_->delta_rot2;

    double delta_rot1_hat = iterators_->delta_rot1_hat;
    double delta_trans_hat = iterators_->delta_trans_hat;

    double delta_rot2_hat = iterators_->delta_rot2_hat;

    
    for(int i = iterators_->begin; i < iterators_->end; i++) {
      //pf_sample_t * sample = pff+ 1;

      double x1, x2, w, r;

      do {
        do {
          r = ((double)rand()) / RAND_MAX;
        } while (r == 0.0);
        x1 = 2.0 * r - 1.0;
        do {
          r = ((double)rand()) / RAND_MAX;
        } while (r == 0.0);
        x2 = 2.0 * r - 1.0;
        w = x1 * x1 + x2 * x2;
      } while (w > 1.0 || w == 0.0);

      double result_one = sqrt(alpha1_ * delta_rot1_noise * delta_rot1_noise + alpha2_ * delta_trans * delta_trans) * x2 * sqrt(-2.0 * log(w) / w);

      delta_rot1 = atan2(sin(delta_rot1), cos(delta_rot1));
      result_one = atan2(sin(result_one), cos(result_one));
      double d1 = delta_rot1 - result_one;
      double d2 = 2 * M_PI - fabs(d1);
      if (d1 > 0) {
        d2 *= -1.0;
      }
      if (fabs(d1) < fabs(d2)) {
        delta_rot1_hat = d1;
      } else {
        delta_rot1_hat = d2;
      }

     

      double result_two = sqrt(alpha3_ * delta_trans * delta_trans + alpha4_ * delta_rot1_noise * delta_rot1_noise + alpha4_ * delta_rot2_noise * delta_rot2_noise) * x2 * sqrt(-2.0 * log(w) / w);
      delta_trans_hat = delta_trans - result_two;


      double result_three =  sqrt(alpha1_ * delta_rot2_noise * delta_rot2_noise + alpha2_ * delta_trans * delta_trans) * x2 * sqrt(-2.0 * log(w) / w);


      delta_rot2 = atan2(sin(delta_rot2), cos(delta_rot2));
      result_three = atan2(sin(result_three), cos(result_three));
      d1 = delta_rot2 - result_three;
      d2 = 2 * M_PI - fabs(d1);
      if (d1 > 0) {
        d2 *= -1.0;
      }
      if (fabs(d1) < fabs(d2)) {
        delta_rot2_hat = d1;
      } else {
        delta_rot2_hat = d2;
      }
    
    pose1 += delta_trans_hat *
      cos(pose2 + delta_rot1_hat);
    pose1 += delta_trans_hat *
      sin(pose2 + delta_rot1_hat);
    pose2 += delta_rot1_hat + delta_rot2_hat;

    }
    
  }, THREAD_NUMBER);


  iterators *iterators_1 = new iterators(0, iterations / 4, alpha1_, alpha2_, alpha3_, alpha4_, set, delta_rot1, delta_rot2, delta_rot1_noise, delta_rot2_noise, delta_trans, delta_trans_hat, delta_rot1_hat, delta_rot2_hat);
  iterators *iterators_2 = new iterators(iterations / 4, (iterations / 4) * 2, alpha1_, alpha2_, alpha3_, alpha4_, set, delta_rot1, delta_rot2, delta_rot1_noise, delta_rot2_noise, delta_trans, delta_trans_hat, delta_rot1_hat, delta_rot2_hat);
  iterators *iterators_3 = new iterators((iterations / 4) * 2, (iterations / 4) * 3, alpha1_, alpha2_, alpha3_, alpha4_, set, delta_rot1, delta_rot2, delta_rot1_noise, delta_rot2_noise, delta_trans, delta_trans_hat, delta_rot1_hat, delta_rot2_hat);
  iterators *iterators_4 = new iterators((iterations / 4) * 3, iterations, alpha1_, alpha2_, alpha3_, alpha4_, set, delta_rot1, delta_rot2, delta_rot1_noise, delta_rot2_noise, delta_trans, delta_trans_hat, delta_rot1_hat, delta_rot2_hat);

  std::pair<odometry_struct, iterators*> * p1 = new std::pair<odometry_struct, iterators*>(settings_,iterators_1);
  std::pair<odometry_struct, iterators*> * p2 = new std::pair<odometry_struct, iterators*>(settings_,iterators_2);
  std::pair<odometry_struct, iterators*> * p3 = new std::pair<odometry_struct, iterators*>(settings_,iterators_3);
  std::pair<odometry_struct, iterators*> * p4 = new std::pair<odometry_struct, iterators*>(settings_,iterators_4);

  auto before = std::chrono::high_resolution_clock::now();
  tasks[0] = p1;
  tasks[1] = p2;  
  tasks[2] = p3;
  tasks[3] = p4;
   
  while(true) 
  {
      if(tasks[0]->first.is_finished)
      {
          break;
      }
  };
  auto after = std::chrono::high_resolution_clock::now();

  return std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT> >(after - before).count();
}

void calculate(std::vector<unsigned long int>& times, const int & steps, const char * str)
{
    unsigned long int min = times[0];
    unsigned long int max = times[0];

    double average = 0.0;
    int length = times.size();

    for(int i = 0; i < length; i++) {
        if(min > times[i])
            min = times[i];
        if(max < times[i])
            max = times[i];
        
        average += times[i];
    } 

    std::cout << "Odometry " << str << " - " << steps << " iterations" << std::endl
              << "min: " << min << " max: " << max <<  " avg: " << average/length << std::endl << std::endl;

}

void init_thread_pool() 
{
    struct sched_param  sched_param_;
    sched_param_.sched_priority = 80;
    pthread_t threads[10];

    cpu_set_t cpuset;
    int cpu_counter = 0;


    for(int i = 0; i < 10; i++)
    {
        int* arg = new int;
        *arg = i;
        tasks[i] = NULL;
        pthread_create(&threads[i], NULL, pool_thread_worker, (void*)arg);
        //pthread_setschedparam(*(threads + i),0, &sched_param_);
        CPU_SET(cpu_counter, &cpuset);
        pthread_setaffinity_np(*(threads + i), sizeof(cpuset), &cpuset);
        CPU_CLR(cpu_counter, &cpuset);
        cpu_counter += 2;
    }

    for(int i = 0; i < 10; i++)
    {
        pthread_detach(threads[i]);
    }  

   

}

}

}

#endif