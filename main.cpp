#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <string>
#include <map>
using namespace std;

map<string, double>pi;
int n, m;
vector<vector<double>>transition;
vector<vector<double>>emission;
vector<vector<double>>alpha;

void readInput(){
  cin >> n >> m;
  transition.resize(n);
  emission.resize(n);  
  for(int i = 0; i < n; ++i){
    transition[i].resize(m);
    emission[i].resize(m);
  }
  
  for(int i = 1; i <= n; ++i){
    double start; cin >> start;
    string name = "y" + to_string(i);
    pi[name] = start;
  }
  for(int i = 0; i < n; ++i){
    for(int j = 0; j < m; ++j){
      cin >> transition[i][j];
    }
  }
  for(int i = 0; i < n; ++i){
    for(int j = 0; j < m; ++j){
      cin >> emission[i][j];
    }
  }
}

pair<double, int> getMaxPrevRow(vector<double>x, int currState){
  double answer = x[0] * transition[0][currState];
  int idx = 0;
  for(int i = 1; i < (int)x.size(); ++i){
    if(x[i]*transition[i][currState] > answer){
    //  cout << "answer before: " << answer << endl;
      answer = x[i]*transition[i][currState];
      //cout << "answer after: " << answer << endl;
      idx = i;
    }
  }
  // cout << "path from y" << idx+1 << " to y" << currState+1 <<" is the max" << endl; 
  
  return {answer, idx};
}

void alphaTable(vector<int>result){
  cout << "OBSERVATION SEQUENCE: ";
  cout << fixed << setprecision(7);
  for(int s : result){
    cout << s << " ";
  } 
  // cout << endl;
  for(int i = 0; i < result.size(); ++i){
    result[i]--;
  }
  // Number of time steps is the observation sequence size
  alpha.resize(result.size());
  for(int i = 0; i < alpha.size(); ++i){
    // Number of states in each vector of alpha
    alpha[i].resize(n);
  }
  cout << endl;
  vector<vector<double>>delta(alpha.size(), vector<double>(alpha[0].size(), 0));
  double firstBig = -1;
  int iDDX = -1;
  for(int j = 1; j <= n; ++j){
    string check = "y" + to_string(j); 
    alpha[0][j-1] = pi[check] * emission[j-1][result[0]];
    delta[0][j-1] = pi[check] * emission[j-1][result[0]];
    if(delta[0][j-1] > firstBig){
      firstBig = delta[0][j-1];
      iDDX = j-1;
    }
  }

  vector<string>mpp;
  mpp.push_back("y" + to_string(iDDX + 1));
  // Outer loop iterates over observable sequence
  for(int i = 1; i < result.size(); ++i){
    string pathid;
    for(int j = 0; j < m; ++j){
      pair<double, int>bigFromPreviousRow = getMaxPrevRow(delta[i-1], j);
      delta[i][j] = bigFromPreviousRow.first *  emission[j][result[i]];   
      double bigSum = 0.0;
      for(int k = 0; k < n; ++k){
        bigSum += transition[k][j] * alpha[i-1][k];
      }
      alpha[i][j] = emission[j][result[i]]*(bigSum);
    }
   // cout << "here" << endl;
      double kk = 0;
      int pop = 0;
      for(int ii = 0; ii < delta[i].size(); ++ii){
        if(delta[i][ii] > kk){
          kk = delta[i][ii];
          pop = ii;
        }
      }
      ///cout << "reached" << endl;
      mpp.push_back("y" + to_string(pop + 1));
   // mpp.push_back(pathid);
  }
  
  double answer = 0;
  cout << endl <<  "PRINTING DELTA TABLE: " << endl;
  for(int i = 0; i < delta.size(); ++i){
    for(int j = 0; j < delta[0].size(); ++j){
      cout << delta[i][j] << " ";
    }
    cout << endl;
  }
  cout << endl;
  cout << "MOST PROBABLE PATH: " << endl;
  for(string s : mpp){
    cout << s << " ";
  }
  string last = mpp[mpp.size() - 1];
  last = last[1];
  cout << " = " << delta[delta.size() - 1][stoi(last) - 1] << endl;
  cout << endl;
  cout << endl;
  cout << "PRINTING ALPHA TABLE: " << endl;
  for(int i = 0; i < alpha.size(); ++i){
    for(int j = 0; j < m; ++j){
      cout << alpha[i][j] << " ";
      if(i == alpha.size() - 1){
        answer += alpha[i][j];
      }
    }
    cout << endl;
  }
  cout << endl;
  cout << "LAST ROW SUM FOR ALPHA: " << answer << endl;
  cout << endl;
}

// Probability of sequence of emissions: alphaTable({sequence})

/* Probability of being in a state at a time given some sequence;

    if time >= sequence length: look at the goal state column of the last row in the alpha table
    
    else: first find the rowsum probability of the short sequence  
    then create all combinations of length (t), then add the goal state column (use alpha table to decide)

    divide the latter probability by the former 
           

*/

int main() {
   readInput();
   vector<vector<int>>data = {
     {1}
   };
   
   int n = data.size();
  for(int i = 0; i < n; i++){
    alphaTable(data[i]);
    cout << "---------------------------------------------------------------------------------------" << endl;
  }
  
}
