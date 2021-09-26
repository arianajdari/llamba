#include<iostream>
#include<iomanip>
#include<math.h>
#include<stdlib.h>
#include<fstream>
#include<vector>
#include<chrono>
#include<mutex>
#include<pthread.h>
#include<sched.h>

#define SIZE 100
#define RANK_100 100
#define RANK_200 200
#define RANK_400 400
#define RANK_800 800

#define RANK_3 3

#define THREAD_POOL 10
#define THREAD_NUMBER 4
#define WIDTH 30

typedef std::milli TIME_MEASUREMENT;

std::mutex mutexes[THREAD_NUMBER];

struct iterators 
{
  iterators(float ** matrix, int i, int j, int rank) : matrix(matrix), i(i), j(j), rank(rank) 
  { }

  float ** matrix;
  int i;
  int j;
  int rank;
};

struct inversion_struct
{
  void (*fn) (iterators*);
  int thread_number;
  bool is_finished;
  bool is_processing;
  
  inversion_struct(void (*fn) (iterators* iterators_), int thread_number) : fn(fn), thread_number(thread_number), is_finished(false), is_processing(false) {};
};

std::pair<inversion_struct, iterators*>* tasks[THREAD_POOL];

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


void inversion3Serial(float ** a)
{
  float ratio;
  int i,j,k;

  for(i=1;i<=RANK_3;i++)
  {
    for(j=1;j<=RANK_3;j++)
    {
        if(i==j)
        {
          a[i][j+RANK_3] = 1;
        }
        else
        {
          a[i][j+RANK_3] = 0;
        }
    }
  }

  /* Applying Gauss Jordan Elimination */
  for(i=1;i<=RANK_3;i++)
  {
    for(j=1;j<=RANK_3;j++)
    {
      if(i!=j)
      {
        ratio = a[j][i]/a[i][i];
        for(k=1;k<=2*RANK_3;k++)
        {
          a[j][k] = a[j][k] - ratio*a[i][k];
        }
      }
    }
  }
  /* Row Operation to Make Principal Diagonal to 1 */
  for(i=1;i<=RANK_3;i++)
  {
    for(j=RANK_3+1;j<=2*RANK_3;j++)
    {
      a[i][j] = a[i][j]/a[i][i];
    }
  }

  std::cout<< std::endl<<"Inverse Matrix is:"<< std::endl;
  for(i=1;i<=RANK_3;i++)
  {
    for(j=RANK_3+1;j<=2*RANK_3;j++)
    {
      std::cout<< a[i][j]<<"\t";
    }
    std::cout<< std::endl;
  }

}
void inversion3Multithread(float **  matrix) 
{
  inversion_struct  settings_ = inversion_struct([](iterators * iterators_) -> void {
    float ratio;
    int i,j,k;

    for(i=iterators_->i;i<=iterators_->j;i++)
    {
      for(j=1;j<=iterators_->j;j++)
      {
          if(i==j)
          {
            iterators_->matrix[i][j+iterators_->rank] = 1;
          }
          else
          {
            iterators_->matrix[i][j+iterators_->rank] = 0;
          }
      }
    }

    for(i=iterators_->i;i<=iterators_->j;i++)
    {
      for(j=iterators_->i;j<=iterators_->rank;j++)
      {
        if(i!=j)
        {
          ratio = iterators_->matrix[j][i]/iterators_->matrix[i][i];
          for(k=1;k<=2*iterators_->j;k++)
          {
            iterators_->matrix[j][k] = iterators_->matrix[j][k] - ratio*iterators_->matrix[i][k];
          }
        }
      }
    }
    /* Row Operation to Make Principal Diagonal to 1 */
    for(i=iterators_->i;i<=iterators_->j;i++)
    {
      for(j=iterators_->rank+1;j<=2*iterators_->rank;j++)
      {
        iterators_->matrix[i][j] = iterators_->matrix[i][j]/iterators_->matrix[i][i];
      }
    }

    
    
  }, THREAD_NUMBER); 

  iterators *iterators_1 = new iterators(matrix, 0, (RANK_3 / 3), RANK_3); 
  iterators *iterators_2 = new iterators(matrix, (RANK_3 / 3), (RANK_3 / 3) * 2, RANK_3);
  iterators *iterators_3 = new iterators(matrix, (RANK_3 / 3) * 2, RANK_3, RANK_3); 

  std::pair<inversion_struct, iterators*> * p1 = new std::pair<inversion_struct, iterators*>(settings_,iterators_1);
  std::pair<inversion_struct, iterators*> * p2 = new std::pair<inversion_struct, iterators*>(settings_,iterators_2);
  std::pair<inversion_struct, iterators*> * p3 = new std::pair<inversion_struct, iterators*>(settings_,iterators_3);
  
  auto before = std::chrono::high_resolution_clock::now();
  tasks[0] = p1;
 
  tasks[1] = p2;
  
  tasks[2] = p3;

  while(true) 
  {
      if( tasks[0]->first.is_finished && tasks[1]->first.is_finished && tasks[2]->first.is_finished)
      {
          break;
      }
  };
  auto after = std::chrono::high_resolution_clock::now();



  std::cout << std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT >>(after-before).count() << std::endl;
}



void inversion100Serial(float ** a)
{
  float ratio;
  int i,j,k;

  for(i=1;i<=RANK_100;i++)
  {
    for(j=1;j<=RANK_100;j++)
    {
        if(i==j)
        {
          a[i][j+RANK_100] = 1;
        }
        else
        {
          a[i][j+RANK_100] = 0;
        }
    }
  }

  /* Applying Gauss Jordan Elimination */
  for(i=1;i<=RANK_100;i++)
  {
    for(j=1;j<=RANK_100;j++)
    {
      if(i!=j)
      {
        ratio = a[j][i]/a[i][i];
        for(k=1;k<=2*RANK_100;k++)
        {
          a[j][k] = a[j][k] - ratio*a[i][k];
        }
      }
    }
  }
  /* Row Operation to Make Principal Diagonal to 1 */
  for(i=1;i<=RANK_100;i++)
  {
    for(j=RANK_100+1;j<=2*RANK_100;j++)
    {
      a[i][j] = a[i][j]/a[i][i];
    }
  }
}
void inversion100OpenMP(float ** a)
{
  float ratio;
  int i,j,k;

  #pragma omp parallel for num_threads(4) collapse(2)
  for(i=1;i<=RANK_100;i++)
  {
    for(j=1;j<=RANK_100;j++)
    {
        if(i==j)
        {
          a[i][j+RANK_100] = 1;
        }
        else
        {
          a[i][j+RANK_100] = 0;
        }
    }
  }

  #pragma omp parallel for num_threads(4) collapse(2)
  for(i=1;i<=RANK_100;i++)
  {
    for(j=1;j<=RANK_100;j++)
    {
      if(i!=j)
      {
        ratio = a[j][i]/a[i][i];
        for(k=1;k<=2*RANK_100;k++)
        {
          a[j][k] = a[j][k] - ratio*a[i][k];
        }
      }
    }
  }
  
  #pragma omp parallel for num_threads(4) collapse(2)
  for(i=1;i<=RANK_100;i++)
  {
    for(j=RANK_100+1;j<=2*RANK_100;j++)
    {
      a[i][j] = a[i][j]/a[i][i];
    }
  }
}
void inversion100Multithread(float **  matrix) 
{
  inversion_struct  settings_ = inversion_struct([](iterators * iterators_) -> void {
    float ratio;
    int i,j,k;

    for(i=iterators_->i;i<=iterators_->j;i++)
    {
      for(j=1;j<=iterators_->j;j++)
      {
          if(i==j)
          {
            iterators_->matrix[i][j+iterators_->rank] = 1;
          }
          else
          {
            iterators_->matrix[i][j+iterators_->rank] = 0;
          }
      }
    }

    for(i=iterators_->i;i<=iterators_->j;i++)
    {
      for(j=iterators_->i;j<=iterators_->rank;j++)
      {
        if(i!=j)
        {
          ratio = iterators_->matrix[j][i]/iterators_->matrix[i][i];
          for(k=1;k<=2*iterators_->j;k++)
          {
            iterators_->matrix[j][k] = iterators_->matrix[j][k] - ratio*iterators_->matrix[i][k];
          }
        }
      }
    }
    /* Row Operation to Make Principal Diagonal to 1 */
    for(i=iterators_->i;i<=iterators_->j;i++)
    {
      for(j=iterators_->rank+1;j<=2*iterators_->rank;j++)
      {
        iterators_->matrix[i][j] = iterators_->matrix[i][j]/iterators_->matrix[i][i];
      }
    }

    
    
  }, THREAD_NUMBER); 

  iterators *iterators_1 = new iterators(matrix, 0, (RANK_100 / 3), RANK_100); 
  iterators *iterators_2 = new iterators(matrix, (RANK_100 / 3), (RANK_100 / 3) * 2, RANK_100);
  iterators *iterators_3 = new iterators(matrix, (RANK_100 / 3) * 2, RANK_100, RANK_100); 

  std::pair<inversion_struct, iterators*> * p1 = new std::pair<inversion_struct, iterators*>(settings_,iterators_1);
  std::pair<inversion_struct, iterators*> * p2 = new std::pair<inversion_struct, iterators*>(settings_,iterators_2);
  std::pair<inversion_struct, iterators*> * p3 = new std::pair<inversion_struct, iterators*>(settings_,iterators_3);
  
  auto before = std::chrono::high_resolution_clock::now();
  tasks[0] = p1;
 
  tasks[1] = p2;
  
  tasks[2] = p3;

  while(true) 
  {
      if( tasks[0]->first.is_finished && tasks[1]->first.is_finished && tasks[2]->first.is_finished)
      {
          break;
      }
  };
  auto after = std::chrono::high_resolution_clock::now();

  std::cout << std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT >>(after-before).count() << std::endl;
}

void inversion200Serial(float ** a)
{
  float ratio;
  int i,j,k;

  for(i=1;i<=RANK_200;i++)
  {
    for(j=1;j<=RANK_200;j++)
    {
        if(i==j)
        {
          a[i][j+RANK_200] = 1;
        }
        else
        {
          a[i][j+RANK_200] = 0;
        }
    }
  }

  /* Applying Gauss Jordan Elimination */
  for(i=1;i<=RANK_200;i++)
  {
    for(j=1;j<=RANK_200;j++)
    {
      if(i!=j)
      {
        ratio = a[j][i]/a[i][i];
        for(k=1;k<=2*RANK_200;k++)
        {
          a[j][k] = a[j][k] - ratio*a[i][k];
        }
      }
    }
  }
  /* Row Operation to Make Principal Diagonal to 1 */
  for(i=1;i<=RANK_200;i++)
  {
    for(j=RANK_200+1;j<=2*RANK_200;j++)
    {
      a[i][j] = a[i][j]/a[i][i];
    }
  }
}
void inversion200OpenMP(float ** a)
{
  float ratio;
  int i,j,k;

  #pragma omp parallel for num_threads(4) collapse(2)
  for(i=1;i<=RANK_200;i++)
  {
    for(j=1;j<=RANK_200;j++)
    {
        if(i==j)
        {
          a[i][j+RANK_200] = 1;
        }
        else
        {
          a[i][j+RANK_200] = 0;
        }
    }
  }

  #pragma omp parallel for num_threads(4) collapse(2)
  for(i=1;i<=RANK_200;i++)
  {
    for(j=1;j<=RANK_200;j++)
    {
      if(i!=j)
      {
        ratio = a[j][i]/a[i][i];
        for(k=1;k<=2*RANK_200;k++)
        {
          a[j][k] = a[j][k] - ratio*a[i][k];
        }
      }
    }
  }

  #pragma omp parallel for num_threads(4) collapse(2)
  for(i=1;i<=RANK_200;i++)
  {
    for(j=RANK_200+1;j<=2*RANK_200;j++)
    {
      a[i][j] = a[i][j]/a[i][i];
    }
  }
}
void inversion200Multithread(float **  matrix) 
{
  inversion_struct  settings_ = inversion_struct([](iterators * iterators_) -> void {
    float ratio;
    int i,j,k;

    for(i=iterators_->i;i<=iterators_->j;i++)
    {
      for(j=1;j<=iterators_->j;j++)
      {
          if(i==j)
          {
            iterators_->matrix[i][j+iterators_->rank] = 1;
          }
          else
          {
            iterators_->matrix[i][j+iterators_->rank] = 0;
          }
      }
    }

    for(i=iterators_->i;i<=iterators_->j;i++)
    {
      for(j=iterators_->i;j<=iterators_->rank;j++)
      {
        if(i!=j)
        {
          ratio = iterators_->matrix[j][i]/iterators_->matrix[i][i];
          for(k=1;k<=2*iterators_->j;k++)
          {
            iterators_->matrix[j][k] = iterators_->matrix[j][k] - ratio*iterators_->matrix[i][k];
          }
        }
      }
    }
    /* Row Operation to Make Principal Diagonal to 1 */
    for(i=iterators_->i;i<=iterators_->j;i++)
    {
      for(j=iterators_->rank+1;j<=2*iterators_->rank;j++)
      {
        iterators_->matrix[i][j] = iterators_->matrix[i][j]/iterators_->matrix[i][i];
      }
    }

    
    
  }, THREAD_NUMBER); 

  iterators *iterators_1 = new iterators(matrix, 0, (RANK_200 / 3), RANK_200); 
  iterators *iterators_2 = new iterators(matrix, (RANK_200 / 3), (RANK_200 / 3) * 2, RANK_200);
  iterators *iterators_3 = new iterators(matrix, (RANK_200 / 3) * 2, RANK_200, RANK_200); 

  std::pair<inversion_struct, iterators*> * p1 = new std::pair<inversion_struct, iterators*>(settings_,iterators_1);
  std::pair<inversion_struct, iterators*> * p2 = new std::pair<inversion_struct, iterators*>(settings_,iterators_2);
  std::pair<inversion_struct, iterators*> * p3 = new std::pair<inversion_struct, iterators*>(settings_,iterators_3);
  
  auto before = std::chrono::high_resolution_clock::now();
  tasks[0] = p1;
 
  tasks[1] = p2;
  
  tasks[2] = p3;

  while(true) 
  {
      if( tasks[0]->first.is_finished && tasks[1]->first.is_finished && tasks[2]->first.is_finished)
      {
          break;
      }
  };
  auto after = std::chrono::high_resolution_clock::now();

  std::cout << std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT >>(after-before).count() << std::endl;
}

void inversion400Serial(float ** a)
{
  float ratio;
  int i,j,k;

  for(i=1;i<=RANK_400;i++)
  {
    for(j=1;j<=RANK_400;j++)
    {
        if(i==j)
        {
          a[i][j+RANK_400] = 1;
        }
        else
        {
          a[i][j+RANK_400] = 0;
        }
    }
  }

  /* Applying Gauss Jordan Elimination */
  for(i=1;i<=RANK_400;i++)
  {
    for(j=1;j<=RANK_400;j++)
    {
      if(i!=j)
      {
        ratio = a[j][i]/a[i][i];
        for(k=1;k<=2*RANK_400;k++)
        {
          a[j][k] = a[j][k] - ratio*a[i][k];
        }
      }
    }
  }
  /* Row Operation to Make Principal Diagonal to 1 */
  for(i=1;i<=RANK_400;i++)
  {
    for(j=RANK_400+1;j<=2*RANK_400;j++)
    {
      a[i][j] = a[i][j]/a[i][i];
    }
  }
}
void inversion400OpenMP(float ** a)
{
  float ratio;
  int i,j,k;

  #pragma omp parallel for num_threads(4) collapse(2)
  for(i=1;i<=RANK_400;i++)
  {
    for(j=1;j<=RANK_400;j++)
    {
        if(i==j)
        {
          a[i][j+RANK_400] = 1;
        }
        else
        {
          a[i][j+RANK_400] = 0;
        }
    }
  }

  #pragma omp parallel for num_threads(4) collapse(2)
  for(i=1;i<=RANK_400;i++)
  {
    for(j=1;j<=RANK_400;j++)
    {
      if(i!=j)
      {
        ratio = a[j][i]/a[i][i];
        for(k=1;k<=2*RANK_400;k++)
        {
          a[j][k] = a[j][k] - ratio*a[i][k];
        }
      }
    }
  }
  
  #pragma omp parallel for num_threads(4) collapse(2)
  for(i=1;i<=RANK_400;i++)
  {
    for(j=RANK_400+1;j<=2*RANK_400;j++)
    {
      a[i][j] = a[i][j]/a[i][i];
    }
  }
}
void inversion400Multithread(float **  matrix) 
{
  inversion_struct  settings_ = inversion_struct([](iterators * iterators_) -> void {
    float ratio;
    int i,j,k;

    for(i=iterators_->i;i<=iterators_->j;i++)
    {
      for(j=1;j<=iterators_->j;j++)
      {
          if(i==j)
          {
            iterators_->matrix[i][j+iterators_->rank] = 1;
          }
          else
          {
            iterators_->matrix[i][j+iterators_->rank] = 0;
          }
      }
    }

    for(i=iterators_->i;i<=iterators_->j;i++)
    {
      for(j=iterators_->i;j<=iterators_->rank;j++)
      {
        if(i!=j)
        {
          ratio = iterators_->matrix[j][i]/iterators_->matrix[i][i];
          for(k=1;k<=2*iterators_->j;k++)
          {
            iterators_->matrix[j][k] = iterators_->matrix[j][k] - ratio*iterators_->matrix[i][k];
          }
        }
      }
    }
    /* Row Operation to Make Principal Diagonal to 1 */
    for(i=iterators_->i;i<=iterators_->j;i++)
    {
      for(j=iterators_->rank+1;j<=2*iterators_->rank;j++)
      {
        iterators_->matrix[i][j] = iterators_->matrix[i][j]/iterators_->matrix[i][i];
      }
    }

    
    
  }, THREAD_NUMBER); 

  iterators *iterators_1 = new iterators(matrix, 0, (RANK_400 / 3), RANK_400); 
  iterators *iterators_2 = new iterators(matrix, (RANK_400 / 3), (RANK_400 / 3) * 2, RANK_400);
  iterators *iterators_3 = new iterators(matrix, (RANK_400 / 3) * 2, RANK_400, RANK_400); 

  std::pair<inversion_struct, iterators*> * p1 = new std::pair<inversion_struct, iterators*>(settings_,iterators_1);
  std::pair<inversion_struct, iterators*> * p2 = new std::pair<inversion_struct, iterators*>(settings_,iterators_2);
  std::pair<inversion_struct, iterators*> * p3 = new std::pair<inversion_struct, iterators*>(settings_,iterators_3);
  
  auto before = std::chrono::high_resolution_clock::now();
  tasks[0] = p1;
 
  tasks[1] = p2;
  
  tasks[2] = p3;

  while(true) 
  {
      if( tasks[0]->first.is_finished && tasks[1]->first.is_finished && tasks[2]->first.is_finished)
      {
          break;
      }
  };
  auto after = std::chrono::high_resolution_clock::now();

  std::cout << std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT >>(after-before).count() << std::endl;
}

void inversion800Serial(float ** a)
{
  float ratio;
  int i,j,k;

  for(i=1;i<=RANK_800;i++)
  {
    for(j=1;j<=RANK_800;j++)
    {
        if(i==j)
        {
          a[i][j+RANK_800] = 1;
        }
        else
        {
          a[i][j+RANK_800] = 0;
        }
    }
  }

  /* Applying Gauss Jordan Elimination */
  for(i=1;i<=RANK_800;i++)
  {
    for(j=1;j<=RANK_800;j++)
    {
      if(i!=j)
      {
        ratio = a[j][i]/a[i][i];
        for(k=1;k<=2*RANK_800;k++)
        {
          a[j][k] = a[j][k] - ratio*a[i][k];
        }
      }
    }
  }
  /* Row Operation to Make Principal Diagonal to 1 */
  for(i=1;i<=RANK_800;i++)
  {
    for(j=RANK_800+1;j<=2*RANK_800;j++)
    {
      a[i][j] = a[i][j]/a[i][i];
    }
  }
}
void inversion800OpenMP(float ** a)
{
  float ratio;
  int i,j,k;

  #pragma omp parallel for num_threads(4) collapse(2)
  for(i=1;i<=RANK_800;i++)
  {
    for(j=1;j<=RANK_800;j++)
    {
        if(i==j)
        {
          a[i][j+RANK_800] = 1;
        }
        else
        {
          a[i][j+RANK_800] = 0;
        }
    }
  }

  #pragma omp parallel for num_threads(4) collapse(2)
  for(i=1;i<=RANK_800;i++)
  {
    for(j=1;j<=RANK_800;j++)
    {
      if(i!=j)
      {
        ratio = a[j][i]/a[i][i];
        for(k=1;k<=2*RANK_800;k++)
        {
          a[j][k] = a[j][k] - ratio*a[i][k];
        }
      }
    }
  }
  
  #pragma omp parallel for num_threads(4) collapse(2)
  for(i=1;i<=RANK_800;i++)
  {
    for(j=RANK_800+1;j<=2*RANK_800;j++)
    {
      a[i][j] = a[i][j]/a[i][i];
    }
  }
}
void inversion800Multithread(float **  matrix) 
{
  inversion_struct  settings_ = inversion_struct([](iterators * iterators_) -> void {
    float ratio;
    int i,j,k;

    for(i=iterators_->i;i<=iterators_->j;i++)
    {
      for(j=1;j<=iterators_->j;j++)
      {
          if(i==j)
          {
            iterators_->matrix[i][j+iterators_->rank] = 1;
          }
          else
          {
            iterators_->matrix[i][j+iterators_->rank] = 0;
          }
      }
    }

    for(i=iterators_->i;i<=iterators_->j;i++)
    {
      for(j=iterators_->i;j<=iterators_->rank;j++)
      {
        if(i!=j)
        {
          ratio = iterators_->matrix[j][i]/iterators_->matrix[i][i];
          for(k=1;k<=2*iterators_->j;k++)
          {
            iterators_->matrix[j][k] = iterators_->matrix[j][k] - ratio*iterators_->matrix[i][k];
          }
        }
      }
    }
    /* Row Operation to Make Principal Diagonal to 1 */
    for(i=iterators_->i;i<=iterators_->j;i++)
    {
      for(j=iterators_->rank+1;j<=2*iterators_->rank;j++)
      {
        iterators_->matrix[i][j] = iterators_->matrix[i][j]/iterators_->matrix[i][i];
      }
    }

    
    
  }, THREAD_NUMBER); 

  iterators *iterators_1 = new iterators(matrix, 0, (RANK_800 / 3), RANK_800); 
  iterators *iterators_2 = new iterators(matrix, (RANK_800 / 3), (RANK_800 / 3) * 2, RANK_800);
  iterators *iterators_3 = new iterators(matrix, (RANK_800 / 3) * 2, RANK_800, RANK_800); 

  std::pair<inversion_struct, iterators*> * p1 = new std::pair<inversion_struct, iterators*>(settings_,iterators_1);
  std::pair<inversion_struct, iterators*> * p2 = new std::pair<inversion_struct, iterators*>(settings_,iterators_2);
  std::pair<inversion_struct, iterators*> * p3 = new std::pair<inversion_struct, iterators*>(settings_,iterators_3);
  
  auto before = std::chrono::high_resolution_clock::now();
  tasks[0] = p1;
 
  tasks[1] = p2;
  
  tasks[2] = p3;

  while(true) 
  {
      if( tasks[0]->first.is_finished && tasks[1]->first.is_finished && tasks[2]->first.is_finished)
      {
          break;
      }
  };
  auto after = std::chrono::high_resolution_clock::now();

  std::cout << std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT >>(after-before).count() << std::endl;
}


void benchmarks3Serial()
{
  float ** matrix3 = new float*[10];

   for(int i = 0; i < 10; i++)
    matrix3[i] = new float[10]; 

  matrix3[1][1]= 1;
  matrix3[1][2]= 1;
  matrix3[1][3]= 3;
  matrix3[2][1]= 1;
  matrix3[2][2]= 3;
  matrix3[2][3]= -3;
  matrix3[3][1]= -2;
  matrix3[3][2]= -4;
  matrix3[3][3]= -4;

  inversion3Serial(matrix3);

}

void benchmarks3Multithreading()
{
  float ** matrix3 = new float*[10];

   for(int i = 0; i < 10; i++)
    matrix3[i] = new float[10]; 

  matrix3[1][1]= 1;
  matrix3[1][2]= 1;
  matrix3[1][3]= 3;
  matrix3[2][1]= 1;
  matrix3[2][2]= 3;
  matrix3[2][3]= -3;
  matrix3[3][1]= -2;
  matrix3[3][2]= -4;
  matrix3[3][3]= -4;

  inversion3Multithread(matrix3);

  for(int i=1;i<=RANK_3;i++)
  {
    for(int j=RANK_3+1;j<=2*RANK_3;j++)
    {
      std::cout<< matrix3[i][j]<<"\t";
    }
    std::cout<< std::endl;
  }


}


void benchmarks100Serial() 
{
  std::ifstream ifs100("matrix_100x100.txt", std::ios::in);

  float **matrix100 = new float*[SIZE];
  for(int i = 0; i < SIZE; i++)
    matrix100[i] = new float[SIZE];

  for(int i = 1; i <= RANK_100; i++)
  {
    for(int j = 1; j <= RANK_100; j++)
    {
      ifs100 >> matrix100[i][j];
    }
  }
 
  auto before = std::chrono::high_resolution_clock::now();
  inversion100Serial(matrix100);
  auto after  = std::chrono::high_resolution_clock::now();
  std::cout << std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT> >(after - before).count() << std::endl;

  for(int i = 0; i < SIZE; i++) {
    delete [] matrix100[i];
  }
  delete [] matrix100;

  ifs100.close();
}
void benchmarks100OpenMP() 
{
  std::ifstream ifs100("matrix_100x100.txt", std::ios::in);

  float **matrix100 = new float*[SIZE];
  for(int i = 0; i < SIZE; i++)
    matrix100[i] = new float[SIZE];

  for(int i = 1; i <= RANK_100; i++)
  {
    for(int j = 1; j <= RANK_100; j++)
    {
      ifs100 >> matrix100[i][j];
    }
  }
 
  auto before = std::chrono::high_resolution_clock::now();
  inversion100OpenMP(matrix100);
  auto after  = std::chrono::high_resolution_clock::now();
  std::cout << std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT> >(after - before).count() << std::endl;

  for(int i = 0; i < SIZE; i++) {
    delete [] matrix100[i];
  }
  delete [] matrix100;

  ifs100.close();
}
void benchmarks100Multithreading() 
{
  std::ifstream ifs100("matrix_100x100.txt", std::ios::in);

  float **matrix100 = new float*[SIZE];
  for(int i = 0; i < SIZE; i++)
    matrix100[i] = new float[SIZE];

  for(int i = 1; i <= RANK_100; i++)
  {
    for(int j = 1; j <= RANK_100; j++)
    {
      ifs100 >> matrix100[i][j];
    }
  }
 
  auto before = std::chrono::high_resolution_clock::now();
  inversion100Multithread(matrix100);
  auto after  = std::chrono::high_resolution_clock::now();
  std::cout << std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT> >(after - before).count() << std::endl;

  for(int i = 0; i < SIZE; i++) {
    delete [] matrix100[i];
  }
  delete [] matrix100;

  ifs100.close();
}

void benchmarks200Serial()
{
  std::ifstream ifs200("matrix_200x200.txt", std::ios::in);

  float **matrix200 = new float*[SIZE];
  for(int i = 0; i < SIZE; i++)
    matrix200[i] = new float[SIZE];

  for(int i = 1; i <= RANK_200; i++)
  {
    for(int j = 1; j <= RANK_200; j++)
    {
      ifs200 >> matrix200[i][j];
    }
  }
 
  auto before = std::chrono::high_resolution_clock::now();
  inversion200Serial(matrix200);
  auto after  = std::chrono::high_resolution_clock::now();
  std::cout << std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT> >(after - before).count() << std::endl;

  for(int i = 0; i < SIZE; i++) {
    delete [] matrix200[i];
  }
  delete [] matrix200;
}
void benchmarks200OpenMP()
{
  std::ifstream ifs200("matrix_200x200.txt", std::ios::in);

  float **matrix200 = new float*[SIZE];
  for(int i = 0; i < SIZE; i++)
    matrix200[i] = new float[SIZE];

  for(int i = 1; i <= RANK_200; i++)
  {
    for(int j = 1; j <= RANK_200; j++)
    {
      ifs200 >> matrix200[i][j];
    }
  }
 
  auto before = std::chrono::high_resolution_clock::now();
  inversion200OpenMP(matrix200);
  auto after  = std::chrono::high_resolution_clock::now();
  std::cout << std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT> >(after - before).count() << std::endl;

  for(int i = 0; i < SIZE; i++) {
    delete [] matrix200[i];
  }
  delete [] matrix200;
}
void benchmarks200Multithreading()
{
  std::ifstream ifs200("matrix_200x200.txt", std::ios::in);

  float **matrix200 = new float*[SIZE];
  for(int i = 0; i < SIZE; i++)
    matrix200[i] = new float[SIZE];

  for(int i = 1; i <= RANK_200; i++)
  {
    for(int j = 1; j <= RANK_200; j++)
    {
      ifs200 >> matrix200[i][j];
    }
  }
 
  auto before = std::chrono::high_resolution_clock::now();
  inversion200Multithread(matrix200);
  auto after  = std::chrono::high_resolution_clock::now();
  std::cout << std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT> >(after - before).count() << std::endl;

  for(int i = 0; i < SIZE; i++) {
    delete [] matrix200[i];
  }
  delete [] matrix200;
}

void benchmarks400Serial()
{
  std::ifstream ifs400("matrix_400x400.txt", std::ios::in);

  float **matrix400 = new float*[SIZE];
  for(int i = 0; i < SIZE; i++)
    matrix400[i] = new float[SIZE];

  for(int i = 1; i <= RANK_400; i++)
  {
    for(int j = 1; j <= RANK_400; j++)
    {
      ifs400 >> matrix400[i][j];
    }
  }
 
  auto before = std::chrono::high_resolution_clock::now();
  inversion400Serial(matrix400);
  auto after  = std::chrono::high_resolution_clock::now();
  std::cout << std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT> >(after - before).count() << std::endl;

  for(int i = 0; i < SIZE; i++) {
    delete [] matrix400[i];
  }
  delete [] matrix400;
}
void benchmarks400OpenMP()
{
  std::ifstream ifs400("matrix_400x400.txt", std::ios::in);

  float **matrix400 = new float*[SIZE];
  for(int i = 0; i < SIZE; i++)
    matrix400[i] = new float[SIZE];

  for(int i = 1; i <= RANK_400; i++)
  {
    for(int j = 1; j <= RANK_400; j++)
    {
      ifs400 >> matrix400[i][j];
    }
  }
 
  auto before = std::chrono::high_resolution_clock::now();
  inversion400OpenMP(matrix400);
  auto after  = std::chrono::high_resolution_clock::now();
  std::cout << std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT> >(after - before).count() << std::endl;

  for(int i = 0; i < SIZE; i++) {
    delete [] matrix400[i];
  }
  delete [] matrix400;
}
void benchmarks400Multithreading()
{
  std::ifstream ifs400("matrix_400x400.txt", std::ios::in);

  float **matrix400 = new float*[SIZE];
  for(int i = 0; i < SIZE; i++)
    matrix400[i] = new float[SIZE];

  for(int i = 1; i <= RANK_400; i++)
  {
    for(int j = 1; j <= RANK_400; j++)
    {
      ifs400 >> matrix400[i][j];
    }
  }
 
  auto before = std::chrono::high_resolution_clock::now();
  inversion400Multithread(matrix400);
  auto after  = std::chrono::high_resolution_clock::now();
  std::cout << std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT> >(after - before).count() << std::endl;

  for(int i = 0; i < SIZE; i++) {
    delete [] matrix400[i];
  }
  delete [] matrix400;
}


void benchmarks800Serial()
{
  std::ifstream ifs800("matrix_800x800.txt", std::ios::in);

  float **matrix800 = new float*[SIZE];
  for(int i = 0; i < SIZE; i++)
    matrix800[i] = new float[SIZE];

  for(int i = 1; i <= RANK_800; i++)
  {
    for(int j = 1; j <= RANK_800; j++)
    {
      ifs800 >> matrix800[i][j];
    }
  }
 
  auto before = std::chrono::high_resolution_clock::now();
  inversion800Serial(matrix800);
  auto after  = std::chrono::high_resolution_clock::now();
  std::cout << std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT> >(after - before).count() << std::endl;

  for(int i = 0; i < SIZE; i++) {
    delete [] matrix800[i];
  }
  delete [] matrix800;
}
void benchmarks800OpenMP()
{
  std::ifstream ifs800("matrix_800x800.txt", std::ios::in);

  float **matrix800 = new float*[SIZE];
  for(int i = 0; i < SIZE; i++)
    matrix800[i] = new float[SIZE];

  for(int i = 1; i <= RANK_800; i++)
  {
    for(int j = 1; j <= RANK_800; j++)
    {
      ifs800 >> matrix800[i][j];
    }
  }
 
  auto before = std::chrono::high_resolution_clock::now();
  inversion800OpenMP(matrix800);
  auto after  = std::chrono::high_resolution_clock::now();
  std::cout << std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT> >(after - before).count() << std::endl;

  for(int i = 0; i < SIZE; i++) {
    delete [] matrix800[i];
  }
  delete [] matrix800;
}
void benchmarks800Multithreading()
{
  std::ifstream ifs800("matrix_800x800.txt", std::ios::in);

  float **matrix800 = new float*[SIZE];
  for(int i = 0; i < SIZE; i++)
    matrix800[i] = new float[SIZE];

  for(int i = 1; i <= RANK_800; i++)
  {
    for(int j = 1; j <= RANK_800; j++)
    {
      ifs800 >> matrix800[i][j];
    }
  }
 
  auto before = std::chrono::high_resolution_clock::now();
  inversion800Multithread(matrix800);
  auto after  = std::chrono::high_resolution_clock::now();
  std::cout << std::chrono::duration_cast<std::chrono::duration<int64_t, TIME_MEASUREMENT> >(after - before).count() << std::endl;

  for(int i = 0; i < SIZE; i++) {
    delete [] matrix800[i];
  }
  delete [] matrix800;
}


int main()
{
  pthread_t threads[10];

  for(int i = 0; i < 10; i++)
  {
      int* arg = new int;
      *arg = i;
      tasks[i] = NULL;
      pthread_create(&threads[i], NULL, pool_thread_worker, (void*)arg);
  }

  for(int i = 0; i < 10; i++)
  {
      pthread_detach(threads[i]);
  }  

  benchmarks3Multithreading();

  //benchmarks100Serial();
  //benchmarks100OpenMP();
  //benchmarks100Multithreading();

  //benchmarks200Serial();
  //benchmarks200OpenMP();
  //benchmarks200Multithreading();

  //benchmarks400Serial();
  //benchmarks400OpenMP();
  //benchmarks400Multithreading();

  //benchmarks800Serial();
  //benchmarks800OpenMP();
  //benchmarks800Multithreading();
  
  while(true) {}
  
  return 0;
}