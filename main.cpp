#include <iostream>
#include <vector>
#include <string>
#include <chrono>   
#include <random>   
#include <algorithm>
#include <fstream>  
#include <map>    
#include <functional> 

using namespace std;

// armazenando as métricas coletadas 
struct Metrics {
    long long comparisons = 0;
    long long swaps = 0;
};

// bubblesort
void bubbleSort(vector<int>& arr, Metrics& metrics) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            metrics.comparisons++;
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                metrics.swaps++;
            }
        }
    }
}

// insertion sort
void insertionSort(vector<int>& arr, Metrics& metrics) {
    int n = arr.size();
    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && (metrics.comparisons++, arr[j] > key)) {
            arr[j + 1] = arr[j];
            metrics.swaps++; 
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

// selection sort
void selectionSort(vector<int>& arr, Metrics& metrics) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        int min_idx = i;
        for (int j = i + 1; j < n; ++j) {
            metrics.comparisons++;
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            swap(arr[i], arr[min_idx]);
            metrics.swaps++;
        }
    }
}

// função auxiliar do merge sort
void merge(vector<int>& arr, int l, int m, int r, Metrics& metrics) {
    int n1 = m - l + 1;
    int n2 = r - m;
    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        metrics.comparisons++;
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        metrics.swaps++;
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++; k++;
        metrics.swaps++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++; k++;
        metrics.swaps++;
    }
}

void mergeSortRecursive(vector<int>& arr, int l, int r, Metrics& metrics) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSortRecursive(arr, l, m, metrics);
        mergeSortRecursive(arr, m + 1, r, metrics);
        merge(arr, l, m, r, metrics);
    }
}
void mergeSort(vector<int>& arr, Metrics& metrics) {
    mergeSortRecursive(arr, 0, arr.size() - 1, metrics);
}

// função auxiliar do quick sort
int partition(vector<int>& arr, int low, int high, Metrics& metrics) {
    int pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; ++j) {
        metrics.comparisons++;
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
            metrics.swaps++;
        }
    }
    swap(arr[i + 1], arr[high]);
    metrics.swaps++;
    return (i + 1);
}

void quickSortRecursive(vector<int>& arr, int low, int high, Metrics& metrics) {
    if (low < high) {
        int pi = partition(arr, low, high, metrics);
        quickSortRecursive(arr, low, pi - 1, metrics);
        quickSortRecursive(arr, pi + 1, high, metrics);
    }
}
void quickSort(vector<int>& arr, Metrics& metrics) {
    quickSortRecursive(arr, 0, arr.size() - 1, metrics);
}

// geração de dados
vector<int> generateRandomVector(int size) {
    vector<int> arr(size);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(1, size * 10);
    for (int i = 0; i < size; ++i) {
        arr[i] = distrib(gen);
    }
    return arr;
}

vector<int> generateReversedVector(int size) {
    vector<int> arr(size);
    for (int i = 0; i < size; ++i) {
        arr[i] = size - i;
    }
    return arr;
}

vector<int> generateNearlySortedVector(int size) {
    vector<int> arr(size);
    for (int i = 0; i < size; ++i) {
        arr[i] = i + 1;
    }

    // inserindo 5% de desordem
    int swaps_to_make = size * 0.05;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, size - 1);

    for (int i = 0; i < swaps_to_make; ++i) {
        int idx1 = distrib(gen);
        int idx2 = distrib(gen);
        swap(arr[idx1], arr[idx2]);
    }
    return arr;
}

int main() {
    ofstream results_file("resultados_ordenacao.csv");
    if (!results_file.is_open()) {
        cerr << "Erro ao abrir o arquivo de resultados!" << endl;
        return 1;
    }
  
    results_file << "Algoritmo,Tipo de Dado,Tamanho,Tempo (ms),Comparacoes,Trocas\n";

    // mapeando nomes de algoritmos para as funções!!
    map<string, function<void(vector<int>&, Metrics&)>> algorithms = {
        {"Bubble Sort", bubbleSort},
        {"Insertion Sort", insertionSort},
        {"Selection Sort", selectionSort},
        {"Merge Sort", mergeSort},
        {"Quick Sort", quickSort}
    };

    // definindo os tamanhos dos vetores
    vector<int> sizes = {100, 1000, 5000, 10000};
    
    // mapeando nomes dos tipos de dados para as funções geradoras
    map<string, function<vector<int>(int)>> data_generators = {
        {"Aleatorio", generateRandomVector},
        {"Quase Ordenado", generateNearlySortedVector},
        {"Inversamente Ordenado", generateReversedVector}
    };

    cout << "Iniciando a analise comparativa de algoritmos de ordenacao..." << endl;

    for (int size : sizes) {
        cout << "\nTestando com vetores de tamanho: " << size << endl;
        for (auto const& [data_type_name, generate_data] : data_generators) {
            cout << "  Tipo de dado: " << data_type_name << endl;
            
            // vetor base
            vector<int> base_arr = generate_data(size);

            for (auto const& [algo_name, sort_function] : algorithms) {
                // criando uma cópia do vetor p/ cada algoritmo
                vector<int> arr_copy = base_arr;
                
                Metrics metrics = {0, 0};

                // medindo tempo de execução
                auto start_time = chrono::high_resolution_clock::now();
                sort_function(arr_copy, metrics);
                auto end_time = chrono::high_resolution_clock::now();
                
                // calculando a duração em milissegundos
                chrono::duration<double, milli> duration = end_time - start_time;
                
                // verificação da ordenação do vetor !!
                if (!is_sorted(arr_copy.begin(), arr_copy.end())) {
                    cerr << "ERRO: O algoritmo " << algo_name << " nao ordenou o vetor corretamente!" << endl;
                }

                // salvando em csv
                results_file << algo_name << ","
                             << data_type_name << ","
                             << size << ","
                             << duration.count() << ","
                             << metrics.comparisons << ","
                             << metrics.swaps << "\n";
                
                cout << "    - " << algo_name << " concluido." << endl;
            }
        }
    }

    results_file.close();
    cout << "\nAnálise concluida! Resultados salvos em 'resultados_ordenacao.csv'" << endl;

    return 0;
}
