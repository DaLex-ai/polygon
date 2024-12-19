#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <chrono>
#include <limits>
#include <algorithm>
#include <cmath>
#include <stack>
#include <unordered_set>

struct Edge {
   double lon;
   double lat;
   double weight;
   Edge(double longitude, double latitude, double w) : lon(longitude), lat(latitude), weight(w) {}
};

struct Node {
   double lon;
   double lat;
   std::vector<Edge> edges;

   Node() : lon(0), lat(0) {}
   Node(double longitude, double latitude) : lon(longitude), lat(latitude) {}
};

class Graph {
public:
   std::unordered_map<std::string, Node> nodes;

   // Хелпер-функция для генерации уникального ключа для узла на основе долготы и широты
   std::string generateKey(double lon, double lat) {
      return std::to_string(lon) + "," + std::to_string(lat);
   }
   
   //Добавляет новый узел в граф
   void addNode(double lon, double lat) {
      std::string key = generateKey(lon, lat);
      nodes.emplace(key, Node(lon, lat)); // Emplace создает узел только если его нет
   }

   //Добавляет рёбра между двумя узлами, при этом автоматически добавляет узлы, если их еще нет в графе.
   void addEdge(double lon1, double lat1, double lon2, double lat2, double weight) {
      addNode(lon1, lat1); 
      addNode(lon2, lat2);

      std::string key1 = generateKey(lon1, lat1);
      nodes[key1].edges.emplace_back(lon2, lat2, weight); // Используем emplace_back для эффективности
   }

   double dijkstra(double lonStart, double latStart, double lonEnd, double latEnd) {
      std::string startKey = generateKey(lonStart, latStart);
      std::string endKey = generateKey(lonEnd, latEnd);

      // Инициализируем расстояния и приоритетную очередь
      std::unordered_map<std::string, double> distances;
      for (const auto& pair : nodes) {
         distances[pair.first] = std::numeric_limits<double>::infinity(); // Устанавливаем бесконечность для каждого узла
      }
      distances[startKey] = 0; // Начальное расстояние равно 0

      auto cmp = [](const auto& left, const auto& right) { return left.second > right.second; };
      std::priority_queue<std::pair<std::string, double>, std::vector<std::pair<std::string, double>>, decltype(cmp)> pq(cmp);
      pq.emplace(startKey, 0); // Добавляем начальный узел в очередь с весом 0

      while (!pq.empty()) {
         auto [currentKey, currentDistance] = pq.top();
         pq.pop();

         if (currentKey == endKey) {
            return currentDistance; // Возвращаем расстояние, когда достигаем конечного ключа
         }

         for (const Edge& edge : nodes[currentKey].edges) {
            std::string neighborKey = generateKey(edge.lon, edge.lat);
            double newDistance = currentDistance + edge.weight;

            if (newDistance < distances[neighborKey]) {
               distances[neighborKey] = newDistance; // Обновляем расстояние
               pq.emplace(neighborKey, newDistance); // Добавляем соседний узел в очередь
            }
         }
      }

      return std::numeric_limits<double>::infinity(); // Путь не найден
   }

   double bfs(double lonStart, double latStart, double lonEnd, double latEnd) {
      std::string startKey = generateKey(lonStart, latStart);
      std::string endKey = generateKey(lonEnd, latEnd);

      if (nodes.find(startKey) == nodes.end() || nodes.find(endKey) == nodes.end()) {
         return std::numeric_limits<double>::infinity(); // Если начальный или конечный узел не найден
      }

      std::unordered_map<std::string, bool> visited;
      std::queue<std::string> queue; // Используем очередь для BFS
      queue.push(startKey);
      visited[startKey] = true; // Помечаем начальный узел как посещенный

      double totalWeight = 0;

      while (!queue.empty()) {
         auto currentKey = queue.front();
         queue.pop();

         for (const Edge& edge : nodes[currentKey].edges) {
            std::string neighborKey = generateKey(edge.lon, edge.lat);
            if (!visited[neighborKey]) {
               visited[neighborKey] = true; // Помечаем соседа как посещенного
               totalWeight += edge.weight; // Считаем вес

               if (neighborKey == endKey) {
                  return totalWeight; // Возвращаем вес, если найдена конечная точка
               }
               queue.push(neighborKey);
            }
         }
      }
      return std::numeric_limits<double>::infinity(); // Путь не найден
   }

   double dfs(double lonStart, double latStart, double lonEnd, double latEnd) {
      std::string startKey = generateKey(lonStart, latStart);
      std::string endKey = generateKey(lonEnd, latEnd);

      if (nodes.find(startKey) == nodes.end() || nodes.find(endKey) == nodes.end()) {
         return std::numeric_limits<double>::infinity(); // Если узлы не найдены
      }

      std::unordered_map<std::string, bool> visited;
      std::stack<std::pair<std::string, double>> stack; // Здесь сохраняем пару (ключ, текущий вес)
      stack.push({startKey, 0}); // Начинаем с начальной точки

      double totalWeight = std::numeric_limits<double>::infinity();

      while (!stack.empty()) {
         auto [currentKey, currentWeight] = stack.top();
         stack.pop();

         if (currentKey == endKey) {
            totalWeight = std::min(totalWeight, currentWeight); // Если нашли путь, обновляем общий вес
            continue; // Перемещаемся дальше
         }

         if (visited[currentKey]) continue; // Если уже посещали эту ноду, пропускаем её

         visited[currentKey] = true; // Помечаем узел как посещенный

         for (const Edge& edge : nodes[currentKey].edges) {
            std::string neighborKey = generateKey(edge.lon, edge.lat);
            if (!visited[neighborKey]) {
               stack.push({neighborKey, currentWeight + edge.weight}); // Добавляем соседей в стек с обновленным весом
            }
         }
      }
      return totalWeight; // Возвращаем общий вес
   }

   double improvedHeuristic(double lon1, double lat1, double lon2, double lat2) {
      // Используем только евклидово расстояние для улучшения оценки
      return std::hypot(lon2 - lon1, lat2 - lat1);
   }

   double astar(double lonStart, double latStart, double lonEnd, double latEnd) {
        std::string startKey = generateKey(lonStart, latStart);
        std::string endKey = generateKey(lonEnd, latEnd);

        std::unordered_map<std::string, double> gScore; 
        std::unordered_map<std::string, double> fScore; 

        auto cmp = [](const std::pair<std::string, double>& left, const std::pair<std::string, double>& right) {
            return left.second > right.second; // Сравнение для приоритетной очереди
        };

        std::priority_queue<std::pair<std::string, double>, std::vector<std::pair<std::string, double>>, decltype(cmp)> openSet(cmp);

        // Инициализация gScore и fScore...
        for (const auto& pair : nodes) {
            gScore[pair.first] = std::numeric_limits<double>::infinity();
            fScore[pair.first] = std::numeric_limits<double>::infinity();
        }
        gScore[startKey] = 0;
        fScore[startKey] = improvedHeuristic(lonStart, latStart, lonEnd, latEnd);
        openSet.emplace(startKey, fScore[startKey]);

        std::unordered_set<std::string> closedSet;

        while (!openSet.empty()) {
            auto [currentKey, currentFScore] = openSet.top();
            openSet.pop();

            if (currentKey == endKey) {
                return gScore[currentKey]; // Возвращаем общий вес пути
            }

            closedSet.insert(currentKey); // Помечаем текущий узел как проверенный

            // Обрабатываем соседей...
            for (const Edge& edge : nodes[currentKey].edges) {
                std::string neighborKey = generateKey(edge.lon, edge.lat);
                if (closedSet.find(neighborKey) != closedSet.end()) {
                    continue; // Пропускаем уже проверенные узлы
                }

                double tentativeGScore = gScore[currentKey] + edge.weight;

                if (tentativeGScore < gScore[neighborKey]) {
                    gScore[neighborKey] = tentativeGScore;
                    fScore[neighborKey] = gScore[neighborKey] + improvedHeuristic(edge.lon, edge.lat, lonEnd, latEnd);

                    // Вставить соседа в openSet, если его там нет.
                    openSet.emplace(neighborKey, fScore[neighborKey]);
                }
            }
        }
        return std::numeric_limits<double>::infinity(); // Путь не найден
    }
};

int main() {
   Graph graph;
   std::ifstream infile("spb_graph.txt");
   std::string line;

   while (std::getline(infile, line)) {
      std::istringstream iss(line);
      std::string nodePart;
      std::getline(iss, nodePart, ':');

      double lon1, lat1;
      char comma;
      std::istringstream(nodePart) >> lon1 >> comma >> lat1;

      std::string edgePart;
      while (std::getline(iss, edgePart, ';')) {
         std::istringstream edgeStream(edgePart);
         double lon2, lat2;
         double weight;
         char colon;
         edgeStream >> lon2 >> comma >> lat2 >> colon >> weight;

         graph.addEdge(lon1, lat1, lon2, lat2, weight); // Добавляем ребро в граф
      }
   }

   // Определяем стартовую и конечную точки
   // Координаты где я живу (они не были найдены в графе, поэтому в примере были использованы другие данные)
   double startLon = 30.340392;
   double startLat = 59.9249903;
   double endLon = 30.3404211;
   double endLat = 59.9247852;
   
   // Проверка наличия начальной и конечной точек в графе
   std::string startKey = graph.generateKey(startLon, startLat);
   std::string endKey = graph.generateKey(endLon, endLat);

   if (graph.nodes.find(startKey) == graph.nodes.end()) {
      std::cout << "Начальная точка не найдена в графе!" << std::endl;
      return 0;
   }

   if (graph.nodes.find(endKey) == graph.nodes.end()) {
      std::cout << "Конечная точка не найдена в графе!" << std::endl;
      return 0;
   }

   // Измеряем время для алгоритма Дейкстры
   auto start = std::chrono::high_resolution_clock::now();
   double dijkstraWeight = graph.dijkstra(startLon, startLat, endLon, endLat);
   auto endDijkstra = std::chrono::high_resolution_clock::now();
   std::chrono::duration<double> elapsedDijkstra = endDijkstra - start;

   // Выводим вес пути Дейкстры
   std::cout << "Алгоритм Дейкстры:" << std::endl;
   if (dijkstraWeight == std::numeric_limits<double>::infinity()) {
      std::cout << "Не удалось найти путь от начальной до конечной точки." << std::endl;
   } else {
      std::cout << "Сумма всех весов рёбер: " << dijkstraWeight << std::endl;
   }
   std::cout << "Время выполнения: " << elapsedDijkstra.count() << " секунд" << std::endl;

   // Измеряем время для алгоритма BFS
   start = std::chrono::high_resolution_clock::now();
   double bfsWeight = graph.bfs(startLon, startLat, endLon, endLat);
   auto endBFS = std::chrono::high_resolution_clock::now();
   std::chrono::duration<double> elapsedBFS = endBFS - start;

   // Выводим вес пути BFS
   std::cout << "Алгоритм BFS:" << std::endl;
   if (bfsWeight == std::numeric_limits<double>::infinity()) {
      std::cout << "Не удалось найти путь от начальной до конечной точки." << std::endl;
   } else {
      std::cout << "Сумма всех весов рёбер: " << bfsWeight << std::endl;
   }
   std::cout << "Время выполнения: " << elapsedBFS.count() << " секунд" << std::endl;

   // Измеряем время для алгоритма DFS
   start = std::chrono::high_resolution_clock::now();
   double dfsWeight = graph.dfs(startLon, startLat, endLon, endLat);
   auto endDFS = std::chrono::high_resolution_clock::now();
   std::chrono::duration<double> elapsedDFS = endDFS - start;

   // Выводим вес пути DFS
   std::cout << "Алгоритм DFS:" << std::endl;
   if (dfsWeight == std::numeric_limits<double>::infinity()) {
      std::cout << "Не удалось найти путь от начальной до конечной точки." << std::endl;
   } else {
      std::cout << "Сумма всех весов рёбер: " << dfsWeight << std::endl;
   }
   std::cout << "Время выполнения: " << elapsedDFS.count() << " секунд" << std::endl;

   // Измеряем время для алгоритма A*
   start = std::chrono::high_resolution_clock::now();
   double astarWeight = graph.astar(startLon, startLat, endLon, endLat);
   auto endAStar = std::chrono::high_resolution_clock::now();
   std::chrono::duration<double> elapsedAStar = endAStar - start;

   // Выводим вес пути A*
   std::cout << "Алгоритм A*:" << std::endl;
   if (astarWeight == std::numeric_limits<double>::infinity()) {
      std::cout << "Не удалось найти путь от начальной до конечной точки." << std::endl;
   } else {
      std::cout << "Сумма всех весов рёбер: " << astarWeight << std::endl;
   }
   std::cout << "Время выполнения: " << elapsedAStar.count() << " секунд" << std::endl;

   return 0;
}
