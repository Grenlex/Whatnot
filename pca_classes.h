#include <vector>
#include <math.h>
#include <algorithm>
#include <string>
#include <cstring>
#include <cstdlib>
#include <exception>
using namespace std;

template <typename T>
class Matrices {
  protected:
  int rows, columns;
  public:
  vector <vector <T>> matrix;
  Matrices(vector <vector <T>> data){
    matrix = data;
    (*this).shape();
  }
  Matrices(){
    matrix = {};
    rows = 0;
    columns = 0;
  }
  int size();
  double trace();
  double determinant(int);
  double frobenius();
  int rank();
  void swap(int, int, int);
  void wbin(const char*);
  void rbin(const char*, int, int);
  void shape();
  Matrices<T> reverse();
  Matrices<T> transpose();
  Matrices<T> adamar(Matrices<T> &);
  template <typename T1> friend Matrices<T1> operator+ (Matrices<T1>, Matrices<T1>);
  template <typename T1> friend Matrices<T1> operator- (Matrices<T1>, Matrices<T1>);
  template <typename T1> friend Matrices<T1> operator* (Matrices<T1>, Matrices<T1>);
  template <typename T1> friend Matrices<T1> operator* (Matrices<T1>, double);
  template <typename T1> friend Matrices<T1> operator/ (Matrices<T1>, double);
  template <typename T1> friend ostream& operator<< (ostream &, Matrices<T1>);
  template <typename T1> friend istream& operator>> (istream &, Matrices<T1> &);
  Matrices<T> operator= (Matrices<T> data){
    matrix = data.matrix;
    rows = data.rows;
    columns = data.columns;
    return *this;
  }
};

template <typename T>
void Matrices<T>::shape(){
    int r = (*this).matrix.size();
    int c = (*this).matrix.at(0).size();
    for (int i = 0; i < r; i++){
        if (c != (*this).matrix.at(i).size()) throw -1;
    }
    rows = r;
    columns = c;
}

template <typename T>
int Matrices<T>::size(){
  return matrix.size();
}

template <typename T>
ostream& operator<< (ostream &out, Matrices<T> m1){
  for (int i = 0; i < m1.matrix.size(); i++){
    for (int j = 0; j < m1.matrix.at(0).size(); j++){
      out << m1.matrix.at(i).at(j) << ' ';
    }
    out << endl;
  }
  return out;
}

template <typename T>
void Matrices<T>::wbin(const char* file){
    T num;
    ofstream stream(file, ios::binary);
    for (int i = 0; i < (*this).matrix.size(); i++){
        for (int j = 0; j < (*this).matrix.at(0).size(); j++){
            num = (*this).matrix.at(i).at(j);
            stream.write(reinterpret_cast<char*>(&num), sizeof(T));
        }
    }
stream.close();
}

template <typename T>
istream& operator>> (istream &in, Matrices<T> &m1){
    string s;
    char* str;
    char* rest;
    double num;
    int i = 0;
    int j = 0;
    while (getline(in, s)){
        m1.matrix.push_back({});
        str = (char*)s.c_str();
        while (1){
            rest = str;
            num = strtod(str, &str);
            if (num == 0 && str == rest) break;
            m1.matrix.at(j).push_back(num);
        }
        j++;
        i=0;
    }
    m1.shape();
    return in;
}

template <typename T>
void Matrices<T>::rbin(const char* file, int x, int y){
    ifstream stream(file, ios::binary);
    stream.seekg(0, ios::beg);
    T num;
    char c;
    for (int i = 0; i < x; i++){
        (*this).matrix.push_back({});
        for (int j = 0; j < y; j++){
            stream.read(&c, sizeof(num));
            num = *(reinterpret_cast<T*>(&c));
            (*this).matrix.at(i).push_back(num);
        }
    }
stream.close();
(*this).shape();
}

template <typename T>
Matrices<T> operator+ (Matrices<T> m1, Matrices<T> m2)
{
    if (m1.rows == m2.rows && m1.columns == m2.columns){
  vector <vector <T>> m3;
  for (int i = 0; i < m1.matrix.size(); i++){
    m3.push_back({});
    for (int j = 0; j < m1.matrix.at(0).size(); j++){
       m3.at(i).push_back(m1.matrix.at(i).at(j) + m2.matrix.at(i).at(j));
    }
  }
  return Matrices(m3);
    }
    else throw 0;
}

template <typename T>
Matrices<T> operator- (Matrices<T> m1, Matrices<T> m2)
{
    if (m1.rows == m2.rows && m1.columns == m2.columns){
  Matrices<T> m3;
  for (int i = 0; i < m1.matrix.size(); i++){
    m3.matrix.push_back({});
    for (int j = 0; j < m1.matrix.at(0).size(); j++){
       m3.matrix.at(i).push_back(m1.matrix.at(i).at(j) - m2.matrix.at(i).at(j));
    }
  }
  m3.shape();
  return m3;
    }
    else throw 0;
}

template <typename T>
Matrices<T> operator* (Matrices<T> m1, Matrices<T> m2)
{
    if (m1.columns == m2.rows){
  Matrices<T> m3;
  double sum = 0;
  for (int i = 0; i < m1.matrix.size(); i++){ //row
    m3.matrix.push_back({});
    for (int j = 0; j < m2.matrix.at(0).size(); j++){ //column
    for (int k = 0; k < m1.matrix.at(0).size(); k++){
      sum += m1.matrix.at(i).at(k) * m2.matrix.at(k).at(j);
    }
    m3.matrix.at(i).push_back(sum);
    sum = 0;
    }
  }
  m3.shape();
  return m3;
    }
    else throw 0;
}

template <typename T>
Matrices<T> operator* (Matrices<T> m1, double a)
{
  Matrices<T> m3;
  for (int i = 0; i < m1.matrix.size(); i++){
    m3.push_back({});
    for (int j = 0; j < m1.matrix.at(0).size(); j++){
       m3.at(i).push_back(m1.matrix.at(i).at(j) * a);
    }
  }
  m3.shape();
  return m3;
}

template <typename T>
Matrices<T> operator/ (Matrices<T> m1, double a)
{
  Matrices<T> m3;
  for (int i = 0; i < m1.matrix.size(); i++){
    m3.matrix.push_back({});
    for (int j = 0; j < m1.matrix.at(0).size(); j++){
       m3.matrix.at(i).push_back(m1.matrix.at(i).at(j) / a);
    }
  }
  m3.shape();
  return m3;
}

template <typename T>
Matrices<T> Matrices<T>::adamar(Matrices<T> &m){
    if ((*this).rows == m.rows && (*this).columns == m.columns){
  vector <vector <T>> m3;
  for (int i = 0; i < matrix.size(); i++){
    m3.push_back({});
    for (int j = 0; j < matrix.at(0).size(); j++){
       m3.at(i).push_back(m.matrix.at(i).at(j) * matrix.at(i).at(j));
    }
  }
  return Matrices(m3);
    }
    else throw 0;
}

template <typename T>
double Matrices<T>::trace(){
    double sum = 0;
    for (int i = 0; i < matrix.size(); i++){
       sum += matrix.at(i).at(i);
  }
  return sum;
}

template <typename T>
class TopTriang : public Matrices<T> {
    public:
    TopTriang (vector <vector <T>> data) : Matrices<T>::Matrices(data){
        for (int i = 0; i < data.size(); i++){
            for (int j = 0; j < i; j++){
               if (data.at(i).at(j) != 0) throw 1; 
            }
        }
    }
};

template <typename T>
class BottomTriang : public Matrices<T> {
    public:
    BottomTriang (vector <vector <T>> data) : Matrices<T>::Matrices(data){
        for (int i = 0; i < data.size(); i++){
            for (int j = i + 1; j < data.at(0).size(); j++){
               if (data.at(i).at(j) != 0) throw 4; 
            }
        }
    }
};

template <typename T>
class Identical : public Matrices<T> {
    public:
    Identical (vector <vector <T>> data) : Matrices<T>::Matrices(data){
        for (int i = 0; i < data.size(); i++){
            for (int j = 0; j < data.at(0).size(); j++){
               if ((i == j && data.at(i).at(j) != 1) || (i != j && data.at(i).at(j) != 0)) throw 5; 
            }
        }
    }
};

template <typename T>
class Symmetrical : public Matrices<T> {
    public:
    Symmetrical (vector <vector <T>> data) : Matrices<T>::Matrices(data){
        for (int i = 0; i < data.size(); i++){
            for (int j = 0; j < data.at(0).size(); j++){
               if (Matrices<T>::rows != Matrices<T>::columns || data.at(i).at(j) != data.at(j).at(i)) throw 6; 
            }
        }
    }
};

template <typename T>
double Matrices<T>::determinant(int n){
    if (rows == columns){
    Matrices<T> submatrix;
    double det = 0;
    if (n == 2)
    return ((matrix[0][0] * matrix[1][1]) - (matrix[1][0] * matrix[0][1]));
       for (int x = 0; x < n; x++) {
          int subi = 0;
          for (int i = 1; i < n; i++) {
             submatrix.matrix.push_back({});
             for (int j = 0; j < n; j++) {
                if (j == x)
                continue;
                submatrix.matrix.at(subi).push_back(matrix[i][j]);
             }
             subi++;
          }
          if (x == 0) submatrix.shape();
          det = det + (pow(-1, x) * matrix[0][x] * submatrix.determinant(n-1));
       }
   return det;
    }
   else throw 0;
}

template <typename T>
double Matrices<T>::frobenius(){
    double sum = 0;
    for (int i = 0; i < matrix.size(); i++){
        for (int j = 0; j < matrix.at(0).size(); j++) sum += pow(matrix.at(i).at(j), 2);
    }
    return sqrt(sum);
}

template <typename T>
void Matrices<T>::swap(int row1, int row2, int col){
    for (int i = 0; i < col; i++)
    {
        int temp = (*this).matrix[row1][i];
        (*this).matrix[row1][i] = (*this).matrix[row2][i];
        (*this).matrix[row2][i] = temp;
    }
}

template <typename T>
int Matrices<T>::rank(){
Matrices<T> tmp;
tmp = (*this);
int rank = tmp.matrix.at(0).size();
for (int row = 0; row < rank; row++){
    if (tmp.matrix[row][row]){
        for (int col = 0; col < tmp.matrix.size(); col++){
            if (col != row){
                double mult = (double)tmp.matrix[col][row]/tmp.matrix[row][row];
                for (int i = 0; i < rank; i++) tmp.matrix[col][i] -= mult * tmp.matrix[row][i];
            }
        }
    }
    else{
    bool reduce = true;
    for (int i = row + 1; i < tmp.matrix.size();  i++){
        if (tmp.matrix[i][row]){
            tmp.swap(row, i, rank);
            reduce = false;
            break ;
        }
    }
    if (reduce){
        rank--;
        for (int i = 0; i < tmp.matrix.size(); i ++) tmp.matrix[i][row] = tmp.matrix[i][rank];
    }
    row--;
    }
}
return rank;
}

template <typename T>
Matrices<T> Matrices<T>::reverse(){
    if(rows == columns){
    double temp;
    int N = (*this).matrix.size();
 
    vector <vector <double>> E;
    for (int i = 0; i < N; i++) E.push_back({});
    Matrices<T> A = (*this);
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++){
            if (i == j)E[i].push_back(1.0);
            else E[i].push_back(0.0);
        }
    for (int k = 0; k < N; k++){
        temp = A.matrix[k][k];
        for (int j = 0; j < N; j++){
            A.matrix[k][j] /= temp;
            E[k][j] /= temp;
        }
        for (int i = k + 1; i < N; i++){
            temp = A.matrix[i][k];
            for (int j = 0; j < N; j++){
                A.matrix[i][j] -= A.matrix[k][j] * temp;
                E[i][j] -= E[k][j] * temp;
            }
        }
    }
    for (int k = N - 1; k > 0; k--){
        for (int i = k - 1; i >= 0; i--){
            temp = A.matrix[i][k];
            for (int j = 0; j < N; j++){
                A.matrix[i][j] -= A.matrix[k][j] * temp;
                E[i][j] -= E[k][j] * temp;
            }
        }
    }
 
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) A.matrix[i][j] = E[i][j];
            
return A;
}
else throw 0;
}

template <typename T>
Matrices<T> Matrices<T>::transpose(){
    int R = (*this).matrix.size();
    int C = (*this).matrix.at(0).size();
    Matrices<T> nm;
    for (int i = 0; i < C; i++) nm.matrix.push_back({});
    for (int i = 0; i < C; i++)
        for (int j = 0; j < R; j++) nm.matrix[i].push_back((*this).matrix[j][i]);
    nm.shape();
    return nm;
}

//////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
class Vectors {
  protected:
  vector <vector <T>> vect;
  int rows, columns;
  public:
  Vectors(vector <vector <T>> data){
    vect = data;
    (*this).shape();
  }
  Vectors(){
    vect = {};
  }
  int size();
  double enorm();
  double maxnorm();
  double angle(Vectors<T> &);
  void wbin(const char*);
  void rbin(const char*, int, int);
  void shape();
  template <typename T1> friend Vectors<T1> operator+ (Vectors<T1> &, Vectors<T1> &);
  template <typename T1> friend Vectors<T1> operator- (Vectors<T1> &, Vectors<T1> &);
  template <typename T1> friend double operator* (Vectors<T1> &, Vectors<T1> &);
  template <typename T1> friend Vectors<T1> operator* (Vectors<T1> &, const int &);
  template <typename T1> friend ostream& operator<< (ostream &, Vectors<T1> &);
  template <typename T1> friend istream& operator>> (istream &, Vectors<T1> &);
};

template <typename T>
void Vectors<T>::shape(){
    int r = (*this).vect.size();
    int c = 1;
    for (int i = 0; i < r; i++){
        if (c != (*this).vect.at(i).size()) throw 2;
    }
    rows = r;
    columns = c;
}

template <typename T>
int Vectors<T>::size(){
  return vect.size();
}

template <typename T>
void Vectors<T>::wbin(const char* file){
    T num;
    ofstream stream(file, ios::binary);
    for (int i = 0; i < (*this).vect.size(); i++){
        for (int j = 0; j < (*this).vect.at(0).size(); j++){
            num = (*this).vect.at(i).at(j);
            stream.write(reinterpret_cast<char*>(&num), sizeof(T));
        }
    }
stream.close();
}

template <typename T>
ostream& operator<< (ostream &out, Vectors<T> &v1){
  for (int i = 0; i < v1.vect.size(); i++){
    for (int j = 0; j < v1.vect.at(0).size(); j++){
      out << v1.vect.at(i).at(j) << ' ';
    }
    out << endl;
  }
  return out;
}

template <typename T>
istream& operator>> (istream &in, Vectors<T> &v1){
    string s;
    char* str;
    char* rest;
    double num;
    int i = 0;
    int j = 0;
    while (getline(in, s)){
        v1.vect.push_back({});
        str = (char*)s.c_str();
        while (1){
            rest = str;
            num = strtod(str, &str);
            if (num == 0 && str == rest) break;
            v1.vect.at(j).push_back(num);
        }
        j++;
        i=0;
    }
    v1.shape();
    return in;
}

template <typename T>
void Vectors<T>::rbin(const char* file, int x, int y){
    ifstream stream(file, ios::binary);
    stream.seekg(0, ios::beg);
    T num;
    char c;
    for (int i = 0; i < x; i++){
        (*this).vect.push_back({});
        for (int j = 0; j < y; j++){
            stream.read(&c, sizeof(num));
            num = *(reinterpret_cast<T*>(&c));
            (*this).vect.at(i).push_back(num);
        }
    }
stream.close();
(*this).shape();
}

template <typename T>
Vectors<T> operator+ (Vectors<T> &v1, Vectors<T> &v2)
{
  if (v1.rows == v2.rows && v1.columns == 1 && v2.columns == 1){
  vector <vector <T>> v3;
  for (int i = 0; i < v1.vect.size(); i++){
    v3.push_back({});
    for (int j = 0; j < v1.vect.at(0).size(); j++){
       v3.at(i).push_back(v1.vect.at(i).at(j) + v2.vect.at(i).at(j));
    }
  }
  return Vectors(v3);
  }
  else throw 3;
}

template <typename T>
Vectors<T> operator- (Vectors<T> &v1, Vectors<T> &v2)
{
  if (v1.rows == v2.rows && v1.columns == 1 && v2.columns == 1){
  vector <vector <T>> v3;
  for (int i = 0; i < v1.vect.size(); i++){
    v3.push_back({});
    for (int j = 0; j < v1.vect.at(0).size(); j++){
       v3.at(i).push_back(v1.vect.at(i).at(j) - v2.vect.at(i).at(j));
    }
  }
  return Vectors(v3);
  }
  else throw 3;
}

template <typename T>
double operator* (Vectors<T> &v1, Vectors<T> &v2)
{
  if (v1.rows == v2.rows && v1.columns == 1 && v2.columns == 1){
  int sum = 0;
  for (int i = 0; i < v1.vect.size(); i++){
    sum += v1.vect.at(i).at(0) * v2.vect.at(i).at(0);
  }
  return sum;
  }
  else throw 3;
}

template <typename T>
Vectors<T> operator* (Vectors<T> &v1, const int &a)
{
  vector <vector <T>> v3;
  for (int i = 0; i < v1.vect.size(); i++){
    v3.push_back({});
    for (int j = 0; j < v1.vect.at(0).size(); j++){
       v3.at(i).push_back(v1.vect.at(i).at(j) * a);
    }
  }
  return Vectors(v3);
}

template <typename T>
double Vectors<T>::enorm(){
    double sum = 0;
    for (int i = 0; i < vect.size(); i++) sum += pow(vect.at(i).at(0), 2);
    return sqrt(sum);
}

template <typename T>
double Vectors<T>::maxnorm(){
    double max = vect.at(0).at(0);
    for (int i = 0; i < vect.size(); i++){
        if (vect.at(i).at(0) > max) max = vect.at(i).at(0);
    }
    return abs(max);
}

template <typename T>
double Vectors<T>::angle(Vectors<T> &v1){
  if (v1.rows == rows && v1.columns == 1 && columns == 1) return ((*this) * v1)/((*this).enorm() * v1.enorm());
  else throw 3;
}

////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
class PCA : public Matrices<T> {
  protected:
  int pc;
  vector <vector <T>> score;
  vector <vector <T>> weight;
  vector <vector <T>> remainder;
  public:
  PCA(vector <vector <T>> data) : Matrices<T>(data){
    if ((*this).rows > (*this).columns) pc = (*this).columns;
    else pc = (*this).rows;
  }
  PCA() : Matrices<T>(){}
  void center();
  double enorm();
  void get_pc();
  void norm();
  void nipals();
  PCA<T> pca_score();
  PCA<T> pca_weight();
  PCA<T> pca_remainder();
  double full_dispersion();
  double explained_dispersion();
  void leverage();
  void variation();
  using Matrices<T>::operator=;
};

template <typename T>
void PCA<T>::get_pc(){
  if ((*this).rows > (*this).columns) pc = (*this).columns;
  else pc = (*this).rows;
}

template <typename T>
void PCA<T>::variation(){
  PCA<T> remainder = (*this).pca_remainder();
  double sum = 0;
  for (int i = 0; i < remainder.rows; i++){
    for (int j = 0; j < remainder.columns; j++){
      sum += remainder.matrix.at(i).at(j);
    }
    cout << sum << endl;
    sum = 0;
  }
}

template <typename T>
void PCA<T>::leverage(){
  PCA<T> score = (*this).pca_score();
  PCA<T> row;
  for (int i = 0; i < (*this).rows; i++){
    row = PCA<T>({score.matrix.at(i)});
    cout << row * ((score.transpose() * score).reverse()) * row.transpose() << endl;
  }
}

template <typename T>
double PCA<T>::full_dispersion(){
  double sum = 0;
  PCA<T> remainder = (*this).pca_remainder();
  for (int i = 0; i < remainder.rows; i++){
    for (int j = 0; j < remainder.columns; j++){
      sum += pow(remainder.matrix.at(i).at(j), 2);
    }
  }
  sum = sum / remainder.rows * remainder.columns;
  return sum;
}

template <typename T>
double PCA<T>::explained_dispersion(){
  double numerator = 0;
  double denominator = 0;
  double sum = 0;
  PCA<T> remainder = (*this).pca_remainder();
  for (int i = 0; i < remainder.rows; i++){
    for (int j = 0; j < remainder.columns; j++){
      numerator += pow(remainder.matrix.at(i).at(j), 2);
    }
  }
  for (int i = 0; i < (*this).rows; i++){
    for (int j = 0; j < (*this).columns; j++){
      denominator += pow((*this).matrix.at(i).at(j), 2);
    }
  }
  sum = 1 - numerator/denominator;
  return sum;
}

template <typename T>
PCA<T> PCA<T>::pca_score(){
  PCA<T> a((*this).score);
  return a;
}

template <typename T>
PCA<T> PCA<T>::pca_weight(){
  PCA<T> a((*this).weight);
  return a;
}

template <typename T>
PCA<T> PCA<T>::pca_remainder(){
  PCA<T> a((*this).remainder);
  return a;
}

template <typename T>
void PCA<T>::center(){
for (int i = 0; i < (*this).columns; i++){
  double average = 0;
  for (int j = 0; j < (*this).rows; j++){
    average += (*this).matrix.at(j).at(i);
  }
  for (int j = 0; j < (*this).rows; j++){
    (*this).matrix.at(j).at(i) -= average / (*this).rows;
  }
}
}

template <typename T>
void PCA<T>::norm(){
for (int i = 0; i < (*this).columns; i++){
  double average = 0;
  double sum = 0;
  for (int j = 0; j < (*this).rows; j++){
    average += (*this).matrix.at(j).at(i);
  }
  for (int j = 0; j < (*this).rows; j++){
    double copy;
    copy = (*this).matrix.at(j).at(i);
    copy -= average / (*this).rows;
    copy = pow(copy, 2);
    sum += copy;
  }
  sum = sqrt(sum / (*this).rows);
  for (int j = 0; j < (*this).rows; j++){
    (*this).matrix.at(j).at(i) = (*this).matrix.at(j).at(i) / sum;
  }
}
}

template <typename T>
double PCA<T>::enorm(){
    double sum = 0;
    for (int i = 0; i < (*this).matrix.size(); i++) sum += pow((*this).matrix.at(i).at(0), 2);
    return sqrt(sum);
}

template <typename T>
void PCA<T>::nipals(){
  double eps = 0.00000001;
  vector <vector <T>> all_matrices;
  PCA<T> E;
  PCA<T> t;
  PCA<T> t_old;
  PCA<T> p;
  PCA<T> d;
  int time_step = 0;

  (*this).get_pc();

  E = (*this);
  E.center();
  E.norm();
  for (int i = 0; i < E.pc; i++){
  for (int j = 0; j < E.rows; j++) t.matrix.push_back({E.matrix.at(j).at(i)});
  t.shape();
  do{
  p = (t.transpose() * E / (t.transpose() * t).matrix.at(0).at(0)).transpose();
  p = p / p.enorm();
  t_old = t;
  t = E * p / (p.transpose() * p).matrix.at(0).at(0);
  d = t_old - t;
  } while (d.enorm() > eps);
  E = E - (t * p.transpose());
  if (time_step == 0){
    for (int i = 0; i < t.rows; i++){
      score.push_back({});
    }

    for (int i = 0; i < p.rows; i++){
      weight.push_back({});
    }
  }
  for (int j = 0; j < p.rows; j++) weight.at(j).push_back(p.matrix.at(j).at(0));
  for (int j = 0; j < t.rows; j++) score.at(j).push_back(t.matrix.at(j).at(0));
  t.matrix = {};
  time_step = 1;
  }
  remainder = E.matrix;
}
