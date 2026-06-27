#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <unordered_map>
#include <set>
#include <cstring>

using namespace std;

// Hash para pares (codigo, color)
struct PairHash {
    size_t operator()(const pair<int, unsigned int>& p) const {
        return hash<long long>()(((long long)p.first << 32) | p.second);
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string modo;
    if (!(cin >> modo)) return 0;

    if (modo == "compress") {
        int n, m;
        cin >> n >> m;
        int total_pixeles = n * m;
        
        vector<unsigned int> img;
        img.reserve(total_pixeles);
        set<unsigned int> colores_unicos;
        
        for (int i = 0; i < total_pixeles; i++) {
            string hex_str;
            cin >> hex_str;
            unsigned int color = stoul(hex_str, nullptr, 16);
            img.push_back(color);
            colores_unicos.insert(color);
        }
        
        vector<unsigned int> diccionario_base(colores_unicos.begin(), colores_unicos.end());
        unordered_map<unsigned int, int> mapa_base;
        for (size_t i = 0; i < diccionario_base.size(); i++) {
            mapa_base[diccionario_base[i]] = i;
        }
        
        // LZW: diccionario mapea secuencias a códigos
        // Usamos pair<int, unsigned int> para (ultimo_codigo, nuevo_color)
        unordered_map<pair<int, unsigned int>, int, PairHash> dict_lzw;
        int proximo_codigo = diccionario_base.size();
        vector<int> codigos_comprimidos;
        codigos_comprimidos.reserve(total_pixeles);
        
        const int MAX_DICT = 2097152;  // 2M para mejor compresión
        
        if (total_pixeles > 0) {
            int codigo_actual = mapa_base[img[0]];
            
            for (int i = 1; i < total_pixeles; i++) {
                unsigned int color = img[i];
                pair<int, unsigned int> clave = {codigo_actual, color};
                
                auto it = dict_lzw.find(clave);
                if (it != dict_lzw.end()) {
                    // La secuencia ya existe, extender
                    codigo_actual = it->second;
                } else {
                    // Nueva secuencia encontrada
                    codigos_comprimidos.push_back(codigo_actual);
                    
                    // Agregar al diccionario si hay espacio
                    if (proximo_codigo < MAX_DICT) {
                        dict_lzw[clave] = proximo_codigo++;
                    }
                    
                    // Reiniciar con el nuevo color
                    codigo_actual = mapa_base[color];
                }
            }
            codigos_comprimidos.push_back(codigo_actual);
        }
        
        // Salida
        vector<int> salida;
        salida.push_back(diccionario_base.size());
        for (unsigned int color : diccionario_base) {
            salida.push_back(color);
        }
        salida.push_back(codigos_comprimidos.size());
        for (int codigo : codigos_comprimidos) {
            salida.push_back(codigo);
        }
        
        cout << salida.size() << "\n";
        for (size_t i = 0; i < salida.size(); i++) {
            cout << salida[i];
            if (i < salida.size() - 1) cout << " ";
        }
        cout << "\n";
        
    } else if (modo == "decompress") {
        int n, m, k;
        cin >> n >> m >> k;
        
        int num_base;
        cin >> num_base;
        vector<unsigned int> diccionario_base(num_base);
        for (int i = 0; i < num_base; i++) {
            cin >> diccionario_base[i];
        }
        
        int num_codigos;
        cin >> num_codigos;
        vector<int> codigos(num_codigos);
        for (int i = 0; i < num_codigos; i++) {
            cin >> codigos[i];
        }
        
        // Diccionario de descompresión: código -> secuencia de colores
        vector<vector<unsigned int>> dict_lzw;
        dict_lzw.reserve(2097152);
        
        for (int i = 0; i < num_base; i++) {
            dict_lzw.push_back({diccionario_base[i]});
        }
        
        int pixeles_impresos = 0;
        bool primera_linea = true;
        
        if (num_codigos > 0) {
            int codigo_anterior = codigos[0];
            
            // Imprimir primera secuencia
            for (unsigned int color : dict_lzw[codigo_anterior]) {
                if (!primera_linea && pixeles_impresos % m == 0) cout << "\n";
                cout << hex << uppercase << setfill('0') << setw(8) << color;
                if (pixeles_impresos % m != m - 1) cout << " ";
                pixeles_impresos++;
                primera_linea = false;
            }
            
            // Procesar resto de códigos
            for (int i = 1; i < num_codigos; i++) {
                int codigo_actual = codigos[i];
                vector<unsigned int> entrada;
                
                if (codigo_actual < (int)dict_lzw.size()) {
                    entrada = dict_lzw[codigo_actual];
                } else if (codigo_actual == (int)dict_lzw.size()) {
                    // Caso especial: código que aún no existe en el diccionario
                    entrada = dict_lzw[codigo_anterior];
                    entrada.push_back(dict_lzw[codigo_anterior][0]);
                } else {
                    // Error - código inválido
                    continue;
                }
                
                // Imprimir entrada
                for (unsigned int color : entrada) {
                    if (!primera_linea && pixeles_impresos % m == 0) cout << "\n";
                    cout << hex << uppercase << setfill('0') << setw(8) << color;
                    if (pixeles_impresos % m != m - 1) cout << " ";
                    pixeles_impresos++;
                    primera_linea = false;
                }
                
                // Agregar nuevo patrón al diccionario
                if (dict_lzw.size() < 2097152) {
                    vector<unsigned int> nueva_entrada = dict_lzw[codigo_anterior];
                    nueva_entrada.push_back(entrada[0]);
                    dict_lzw.push_back(nueva_entrada);
                }
                
                codigo_anterior = codigo_actual;
            }
            
            if (pixeles_impresos % m != 0) cout << "\n";
        }
    }
    return 0;
}
