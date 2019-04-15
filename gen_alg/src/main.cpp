#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <exception>
#include <algorithm>
#include <map>
#include <string>

using namespace std;

static random_device rd;
static default_random_engine rand_eng(rd());

//double func_egnholdera(int x, int y)
//{
//  double first_root = sqrt(abs(x/2.0 + y + 47));
//  double second_root = sqrt(abs(x - (y + 47)));
//  return -(y+47)*sin(first_root) - x*sin(second_root);
//}

double f(int x)
{
  return pow(x - 100.6, 2);
}

struct Ind
{
  Ind(const size_t size)
  {
    uniform_int_distribution<int> r(0,1);
    for(size_t i = 0; i < size; ++i)
    {
      genom.push_back(r(rand_eng));
    }
  }
  Ind(const vector<bool> & o)
  {
    genom = o;
  }
  int operator()()
  {
    return to_int();
  }

  Ind crossingover_with(Ind & other) const
  {
    if(genom.size() != other.genom.size())
    {
      throw invalid_argument("genom sizes are different");
    }
    vector<bool> v;
    uniform_int_distribution<long> r(0, genom.size() - 1);
    long clip = r(rand_eng);

    v.insert(v.begin(), genom.begin(), genom.begin() + clip);
    v.insert(v.begin() + clip, other.genom.begin() + clip, other.genom.end());
    return Ind(v);
  }

  void mutate(const double prob)
  {
    uniform_real_distribution<double> r(0,1);
    double rr = r(rand_eng);
    if(rr < prob)
    {
      //                                 чтоб не задеть знак
      uniform_int_distribution<size_t> n(1, genom.size() - 1);
      size_t gene_ind = n(rand_eng);
      genom.at(gene_ind) = !genom.at(gene_ind);
    }
  }

  int to_int() const
  {
    int res = 0;
    uint pow_ = 0;
    for(auto rit = genom.rbegin(); rit != genom.rend() - 1; ++rit)
    {
      res += static_cast<int>((*rit) * pow(2, pow_));
      pow_++;
    }
    res = *genom.begin() == 1 ? -res : res;
    return res;
  }

  string to_bin() const {
    string s;
    for(auto gene : genom)
    {
      s += to_string(gene);
    }
    return s;
  }

  vector<bool> genom;
};

void sortPopulation(vector<Ind> & population)
{
  uint population_size = population.size();
  bool sorted = false;
  while (!sorted)
  {
    sorted = true;
    for(size_t i = 0; i < population_size - 1; ++i)
    {
      if(f(population[i].to_int()) < f(population[i + 1].to_int()))
      {
        iter_swap(population.begin() + i, population.begin() + i + 1);
        sorted = false;
      }
    }
  }
}

uint max_duplicates(const vector<Ind> v)
{
  map<string, uint> m;
  using pair_t = decltype (m)::value_type;
  for_each(v.begin(), v.end(), [&m](Ind val){m[val.to_bin()]++;});
  auto res = max_element(m.begin(), m.end(), [](const pair_t i1, const pair_t i2){ return i1.second < i2.second; });
  return res->second;
}

int main()
{
  uint population_size = 15;
  uint genom_size = 9;
  uint tournir_size = 2;
  double mutation_probability = 0.9;
  vector<Ind> population;

//  Ind i1 {{true, false}};
//  Ind i2 {{false, true}};
//  Ind i3 = i1.crossingover_with(i2);
//  cout << i3.to_bin() << endl;
//  cout << i3.to_int() << endl;

//  return 0;

  for(size_t i = 0; i < population_size; ++i)
  {
    population.push_back(Ind(genom_size));
  }

  for(auto i : population)
  {
    cout << i.to_int() << " " << f(i.to_int()) << endl;
  }

  uniform_int_distribution<size_t> r(0, population_size - 1);
  uint counter = 0;
  while(counter++ <= 20 && max_duplicates(population) != population_size)
  {
    vector<Ind> new_population;
    while (new_population.size() != population.size())
    {
      vector<Ind> turnir;
      while (turnir.size() != tournir_size)
      {
        Ind & i1 = population[r(rand_eng)];
        Ind & i2 = population[r(rand_eng)];
        Ind & winner = (f(i1()) < f(i2())) ? i1 : i2;
        turnir.push_back(winner);
      }

      Ind n = turnir.begin()->crossingover_with(*(turnir.begin() + 1));

      n.mutate(mutation_probability);

      new_population.push_back(n);
    }
    vector<Ind> tmp_population = population;
    for(auto &i:new_population)
    {
      tmp_population.push_back(i);
    }

    sortPopulation(tmp_population);

    vector<Ind> qwe {tmp_population.rbegin(), tmp_population.rbegin() + population.size()};
    population = qwe;
  }

  sortPopulation(population);
  cout << "------------" << endl;
  cout << "counter: " << counter << endl;
//  for(auto i : population)
//  {
//    cout << i.to_int() << " " << f(i.to_int()) << endl;
//  }

  cout << "leader" << endl;
  cout << population.back().to_bin() << endl;
  cout << population.back().to_int() << endl;
  cout << f(population.back().to_int()) << endl;
}
