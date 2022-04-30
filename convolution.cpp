#include <iostream>

using namespace std;

int main(){
	
	int image[5][5] = {{1,2,1,0,2},{2,0,0,1,0},{1,0,2,1,0},{0,1,0,2,1},{0,2,1,0,2}};
	int filter[3][3] = {{1,0,1},{0,1,0},{1,0,1}};
	int output[5][5];
	
	//iterate source image
	for(int i = 0; i < 5; i++){
		for(int j = 0; j < 5; j++){
		
			int sum = 0;
			//iterate filter
			for(int k = -1; k <= 1; k++){
				for(int m = -1; m <= 1; m++){
					//image indexes
					int r = k+i;
					int c = j+m;
					
					//bounds handle
					if(r < 0 || c < 0 || r >= 5 || c >= 5){
					
						continue;
					}
					else{
					
						sum += image[r][c] * filter[k+1][m+1];
					}
					
				}
			
			}
			output[i][j] = sum;
		
		}
	
	}
	
		
	for(int i = 0; i < 5; i++){
		for(int j = 0; j < 5; j++){
		
			cout<< output[i][j] << " ";
		}
		cout<<endl;
	}
	

	return 0;
}
