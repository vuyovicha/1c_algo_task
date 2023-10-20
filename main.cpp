#include <iostream>
#include <vector>
#include <string>

class Solution {
  struct Point {
   public:
    int x;
    int y;

    Point() : x(0), y(0) {}

    Point(int x, int y) : x(x), y(y) {}

    Point &operator+=(const Point &other) {
      x += other.x;
      y += other.y;
      return *this;
    }

    Point &operator-=(const Point &other) {
      x -= other.x;
      y -= other.y;
      return *this;
    }

    Point operator+(const Point &other) const {
      return {x + other.x, y + other.y};
    }

    Point operator-(const Point &other) const {
      return {x - other.x, y - other.y};
    }

    bool operator==(const Point &other) const {
      return x == other.x && y == other.y;
    }

    Point(const Point &other) {
      x = other.x;
      y = other.y;
    }

  };

  enum directions_indexes {
    right = 0, down = 1, left = 2, up = 3
  };

  enum bot_actions {
    step = 1, turn = 2, look_around = 3, finish = 4, double_turn = 5
  };

  enum cell_states {
    wall = 0, visited = 1, unvisited = 2, unknown = 3
  };

  const std::vector<Point> directions = {{1, 0}, {0, -1}, {-1, 0}, {0, 1}};
  const int size = 2 * 10000;

  Point current_point;
  Point direction_point;
  int step_time = 0;
  int turn_time = 0;
  int look_around_time = 0;
  int look_around_radius = 0;
  int time_spent = 0;
  int current_direction_index;
  std::vector<std::vector<int>> maze;

  int decideAction() {
    int neighbour_state = maze[current_point.y + directions[current_direction_index].y][current_point.x
        + directions[current_direction_index].x];
    if (neighbour_state == unknown) {
      return look_around;
    }
    if (neighbour_state == unvisited) {
      current_point += directions[current_direction_index];
      return step;
    }
    if (neighbour_state == wall) {
      current_direction_index = (current_direction_index + 1) % (int) directions.size();
      return turn;
    }
    if (newPathAvailable()) {
      current_direction_index = (current_direction_index + 1) % (int) directions.size();
      return turn;
    } else {
      current_direction_index = (current_direction_index + 2) % (int) directions.size();
      return double_turn;
    }
    return finish;
  }

  bool newPathAvailable() {
    for (const auto &direction : directions) {
      if (maze[current_point.y + direction.y][current_point.x + direction.x] == unvisited) {
        return true;
      }
    }
    return false;
  }

  int getStartDirectionIndex() {
    Point direction = direction_point - current_point;
    for (int i = 0; i < directions.size(); i++) {
      if (direction == directions[i]) {
        return i;
      }
    }
  }

  static int convertCharToState(char symbol) {
    if (symbol == '#') {
      return wall;
    }
    return unvisited;
  }

  void updateMaze() {
    Point visible_point = current_point - Point(look_around_radius, look_around_radius);
    for (int y = 0; y < (2 * look_around_radius + 1); y++) {
      std::string line;
      std::cin >> line;
      for (int x = 0; x < (2 * look_around_radius + 1); x++) {
        if (maze[visible_point.y + y][visible_point.x + x] == unknown
            || maze[visible_point.y + y][visible_point.x + x] == wall) {
          maze[visible_point.y + y][visible_point.x + x] = convertCharToState(line[x]);
        }
      }
    }
  }

 public:
  Solution() {
    std::cin >> current_point.x >> current_point.y;
    std::cin >> direction_point.x >> direction_point.y;
    std::cin >> step_time >> turn_time;
    std::cin >> look_around_time >> look_around_radius;
    std::vector<std::vector<int>> new_maze(size, std::vector<int>(size, unknown));
    maze = new_maze;
  }

  void Play() {
    bool finished_flag = false;
    current_direction_index = getStartDirectionIndex();
    current_point += {size / 2, size / 2};
    maze[current_point.y][current_point.x] = visited;
    bool begin = true;
    while (!finished_flag) {
      int decided_action = decideAction();
      if (begin) {
        decided_action = look_around;
        begin = false;
      }
      if (decided_action == step) {
        int successful_step_flag;
        std::cout << step << std::endl;
        std::cin >> successful_step_flag;
        time_spent += step_time;
      } else if (decided_action == turn) {
        int successful_turn_flag;
        std::cout << turn << std::endl;
        std::cin >> successful_turn_flag;
        time_spent += turn_time;
      } else if (decided_action == look_around) {
        std::cout << look_around << std::endl;
        updateMaze();
        time_spent += look_around_time;
      } else if (decided_action == finish){
        finished_flag = true;
        std::cout << finish << std::endl;
        std::cout << time_spent << std::endl;
      } else {
        int successful_turn_flag;
        std::cout << turn << std::endl;
        std::cin >> successful_turn_flag;
        std::cout << turn << std::endl;
        std::cin >> successful_turn_flag;
        time_spent += 2 * turn_time;
      }
    }
  }
};

int main() {
  Solution solution;
  solution.Play();
  return 0;
}
