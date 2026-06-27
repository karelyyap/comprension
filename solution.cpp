#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <map>
#include <set>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string modo;
    cin >> modo;

    if (modo == "compress") {
        int n, m;
        cin >> n >> m;
        int total = n * m;
        
        vector<unsigned int> pixeles;
        set<unsigned int> colores_unicos;
        
        for (int i = 0; i < total; i++) {
            string hex;
            cin >> hex;
            unsigned int color = stoul(hex, nullptr, 16);
            pixeles.push_back(color);
            colores_unicos.insert(color);
        }
        
        vector<unsigned int> diccionario(colores_unicos.begin(), colores_unicos.end());
        map<unsigned int, int> color_a_idx;
        for (size_t i = 0; i < diccionario.size(); i++) {
            color_a_idx[diccionario[i]] = i;
        }
        
        cout << diccionario.size() << "\n";
        for (size_t i = 0; i < diccionario.size(); i++) {
            cout << diccionario[i];
            if (i < diccionario.size() - 1) cout << " ";
        }
        for (int i = 0; i < total; i++) {
            cout << " " << color_a_idx[pixeles[i]];
        }
        cout << "\n";
        
    } else if (modo == "decompress") {
        int n, m, k;
        cin >> n >> m >> k;
        
        vector<unsigned int> colores(k);
        vector<int> indices(n * m);
        
        for (int i = 0; i < k + n * m; i++) {
            if (i < k) {
                cin >> colores[i];
            } else {
                cin >> indices[i - k];
            }
        }
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                int idx = i * m + j;
                cout << hex << uppercase << setfill('0') << setw(8) << colores[indices[idx]];
                if (j < m - 1) cout << " ";
            }
            cout << "\n";
        }
    }
    
    return 0;
}
