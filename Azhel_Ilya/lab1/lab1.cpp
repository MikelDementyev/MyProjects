#include <iostream>
#include <vector>

using namespace std;

void div_by_2(int N) // Если квадрат кратен двум
{
    cout << 4 << endl;
    cout << 1 << " " << 1 << " " << N/2 << endl;
    cout << 1 << " " << N/2+1 << " " << N/2 << endl;
    cout << N/2+1 << " " << 1 << " " << N/2 << endl;
    cout << N/2+1 << " " << N/2+1 << " " << N/2 << endl;
}

void div_by_3(int N) // Если квадрат кратен трем
{
    cout << 6 << endl;
    cout << 1 << " " << 1 << " " << N/3 << endl;
    cout << 1 << " " << N/3+1 << " " << N/3 << endl;
    cout << 1 << " " << N/3*2+1 << " " << N/3 << endl;
    cout << N/3+1 << " " << 1 << " " << N/3 << endl;
    cout << N/3*2+1 << " " << 1 << " " << N/3 << endl;
    cout << N/3+1 << " " << N/3+1 << " " << N/3*2 << endl;
}

void div_by_5(int N) // Если квадрат кратен пяти
{
    cout << 8 << endl;
    cout << 1 << " " << 1 << " " << N/5*2 << endl;
    cout << 1 << " " << N/5*2+1 << " " << N/5*3 << endl;
    cout << N/5*2+1 << " " << 1 << " " << N/5 << endl;
    cout << N/5*2+1 << " " << N/5+1 << " " << N/5 << endl;
    cout << N/5*3+1 << " " << 1 << " " << N/5*2<< endl;
    cout << N/5*3+1 << " " << N/5*2+1 << " " << N/5 << endl;
    cout << N/5*4+1 << " " << N/5*2+1 << " " << N/5 << endl;
    cout << N/5*3+1 << " " << N/5*3+1 << " " << N/5*2 << endl;
}

bool free_point(vector< vector<int> > field, int &x, int &y)
{
    for(int i = 0; i<field.size(); i++)
    {
        for (int j = 0; j<field.size(); j++)
        {
            if(field[i][j] == 0)
            {
                x = j;
                y = i;
                return true;
            }
        }
    }
    return false;
}

void field_painting(vector< vector<int> > &field, int x, int y, int w, int colour) // Закрашивание квадрата
{
    for(int i = x; i<x+w; i++)
    {
        for(int j = y; j<y+w; j++)
            field[i][j] = colour;
    }
}

int max_square(vector< vector<int> > field, int x, int y)
{
    int w_str = 0;
    int w_col = 0;
    
    for(int j = x; j<field.size(); j++)
    {
        w_str++;
        
        if(field[y][j] == 1)
            break;
    }
    
    for(int i = y; i<field.size(); i++)
    {
        w_col++;
        
        if(field[i][x] == 1)
            break;
    }
    
    if(w_str == w_col) return w_str;
    if(w_str > w_col) return w_col;
    if(w_str < w_col) return w_str;
}

void backtracking(vector< vector<int> > field,int w,int N,int x,int y, int &min_size,int colour,int* result)
{
    if(colour >= min_size)
        return;
    
    for(int i = x; i<x+w; i++)
    {
        for(int j = y; j<y+w; j++)
        {
            if(field[i][j] != 0)
                return;
        }
    }
    
    colour++;
    
    field_painting(field, x, y, w, colour);
    
    int temp_x = x;
    int temp_y = y;
    
    
    if(free_point(field, y, x) == false)
    {
        if(min_size > colour)
        {
            int count = 0;
            int sq_colour = 1;
            int ind = 0;
            
            for(int i = 0; i<N; i++)
            {
                for(int j = 0; j<N; j++)
                {
                    if(field[i][j] == sq_colour)
                    {
                        result[ind++] = j;
                        result[ind++] = i;
                        count = j;
                        
                        while(field[i][count] == sq_colour)
                        {
                            count++;
                        }
                        result[ind++] = count-j;
                        sq_colour++;
                    }
                }
            }
            min_size = colour;
        }
    }
    
    else
        free_point(field, y, x);
    
    for(int i = max_square(field, x, y); i>0; i--)
        backtracking(field,i,N,x,y,min_size,colour,result);
    
    
    for(int i = temp_y; i<temp_y+w; i++)
        for(int j = temp_x; j<temp_x+w; j++)
            field[i][j] = 0;
}

int main()
{
    int N;
    cin >> N;
    
    /*    if(N % 2 == 0)
     {
     div_by_2(N);
     return 0;
     }
     
     if(N % 3 == 0)
     {
     div_by_3(N);
     return 0;
     }
     
     if(N % 5 == 0)
     {
     div_by_5(N);
     return 0;
     }
     */
    
    int min = 20;
    int* result = new int[min*3];
    
    vector< vector<int> > field(N, vector<int>(N, 0));
    
    int x = 0;
    int y = 0;
    //int w = N/2-1;
    int w;
    int colour = 0;
    
    w = (N+1)/2;
    backtracking(field,w,N,x,y,min,colour,result);
    
    w = N*2/3;
    backtracking(field,w,N,x,y,min,colour,result);
    
    w = N*3/5;
    backtracking(field,w,N,x,y,min,colour,result);
    
    //     while(w <= N/2+1)
    //    {
    //        backtracking(field,w,N,x,y,min,colour,result);
    //        w++;
    //    }
    
    cout << min << endl;
    
    for(int i = 0; i<min*3; i++)
    {
        cout << result[i+1]+1 << " " << result[i]+1 << " " << result[i+2] << endl;
        i += 2;
    }
    
    
    return 0;
}
