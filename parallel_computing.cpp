#include <iostream>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

using namespace std;

#define MAX 7  
#define MAX2 4 
#define MAX_THREAD 5 

struct Result { 
    int i;
    int j;
    int res;
};

int M1[MAX][MAX];    
int M2[MAX2][MAX2]; 
int MR[MAX][MAX];   

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;   

int step_i = 0; 

void* multi(void* arg) { 
    
    Result* result = reinterpret_cast<Result*>(arg); 
    
    int i, j, res; 
    
    while (1) { 
        pthread_mutex_lock(&mutex);  
        i = step_i++; 
        pthread_mutex_unlock(&mutex); 
        
        if (i >= MAX) 
            break;

        for (j = 0; j < MAX; j++) { 
            res = 0;
            for (int k = 0; k < MAX2; k++) { 
                res += M1[i][k] * M2[k][j]; 
            }

            pthread_mutex_lock(&mutex);  
            MR[i][j] = res; 
            result->i = i;  
            result->j = j;  
            result->res = res; 
            cout << "Thread [" << result->i << "][" << result->j << "] multiplication result is: " << result->res << endl; 

            for (int x = 0; x < MAX; x++) {
                for (int y = 0; y < MAX; y++)
                    cout << MR[x][y] << " ";
                cout << endl;
            }
            pthread_mutex_unlock(&mutex); 
        }
    }
    return NULL;
}

int main() {
    srand(time(NULL));  

    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            M1[i][j] = rand() % 10;    
            MR[i][j] = -1;   
        }
    }

    for (int i = 0; i < MAX2; i++) {
        for (int j = 0; j < MAX2; j++) {
            M2[i][j] = rand() % 10;  
        }
    }

    cout << "Matrix M1:" << endl;
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++)
            cout << M1[i][j] << " ";
        cout << endl;
    }

    cout << "\nMatrix M2:" << endl;
    for (int i = 0; i < MAX2; i++) {
        for (int j = 0; j < MAX2; j++)
            cout << M2[i][j] << " ";
        cout << endl;
    }

    cout << "\nInitial Multiplication Result:" << endl;
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++)
            cout << MR[i][j] << " ";
        cout << endl;
    }

    pthread_t threads[MAX_THREAD]; 
    Result result[MAX_THREAD];     

    for (int j = 0; j < MAX; j++) {   
        for (int i = 0; i < MAX_THREAD; i++) { 
            result[i].i = -1;  
            result[i].j = -1;
            result[i].res = -1;  
            result[i].j = j;
            pthread_create(&threads[i], NULL, multi, &result[i]); 
        }

        for (int i = 0; i < MAX_THREAD; i++) 
            pthread_join(threads[i], NULL); 
    }
   return 0;
}
