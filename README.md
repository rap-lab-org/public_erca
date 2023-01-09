# Enhanced Resource Constrained A* (ERCA*)

This work addresses a Resource Constrained Shortest Path Problem (RC-SPP) on a graph where the objective is to find a min-cost start-goal path while ensuring that the resource consumed along the path do not exceed the given limits.
This repo provides a C++ implementation of Enhanced Resource Constrained A\* (ERCA\*) Algorithm, which is guaranteed to find an optimal solution path.

The code is distributed for academic and non-commercial use.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

## Requirements

* We use CMake (3.16.3) and Make (4.2.1) to compile the code. Lower or higher version may also work.

## Project Structure

* `README.md` - This file
* `source/` - Contains the path planning source code
* `test/` - Contains example code for path planning algorithms
* `include/` - Contains header files
* `data/` - Contains sample graph files and sample result files

## Instructions:

### Installation

* Clone this repo
* Compile this repo
  * `mkdir build`
  * `cd build`
  * `cmake .. -DCMAKE_BUILD_TYPE=release` (You can specify the build type you like by adding different args)
  * `make`

### Command-Line Interface (CLI)

* Run example via command-line interface (CLI)
  * `cd build/`
  * `./run_erca 1 5 60 3 ../data/ex1-c1.gr ../data/ex1-c2.gr ../data/ex1-c3.gr 10 9 ../data/result.txt`
  * Runs ERCA\* on 3-cost graph (edge weights detailed in `data/ex1-c1.txt`, `data/ex1-c2.txt`, `data/ex1-c3.txt`) to find solutions from node 1 to node 5 with a 60 second time limit, and saves results into `data/result.txt`. The cost to be minimized is defined in `data/ex1-c1.txt` while the two types of resource are defined in `data/ex1-c2.txt` and `data/ex1-c3.txt`. The resource limits are 10 and 9 respectively.
* General usage of the command-line interface
  * `./run_erca (arg1 v_start) (arg2 v_dest) (arg3 time_limit) (arg4 M) (arg5 graph1_file_path) (arg6 graph2_file_path) ... ((arg(M+4) graphM_file_path)) (arg(M+4+1) resource limit 1) (arg(M+4+2) resource limit 2) ... (arg(M+4+M-1) resource limit M-1) (arg(M+4+M) result_path)`
  * arg1 v_start = the starting node
  * arg2 v_dest = the destination node
  * arg3 time_limit = the time limit for ERCA\*
  * arg4 M = the number of criteria (including both the minimizing objective and all types of resource) for the input instance
  * arg5~arg(M+4) = the paths to M files that describe the graph, where each file contains the edge weights for one type of edge cost/resource in the graph (details about file structure are specified below)
  * arg(M+4+1)~arg(M+4+M-1) = the resource limits
  * arg(M+4+M) = the result file path
* For help info `./run_erca -h` or `./run_emoa --help`


### Graph file specification

Graphs are directed graphs labeled from 1~*n*, where *n* is the number of vertices.

If parallel edges are specified in the graph file, only the lexico-smallest edge is used.

Graph files must follow the same format described by [DIMACS](http://www.diag.uniroma1.it//~challenge9/format.shtml#graph).

Graph files used in the same run must correspond to each other line-by-line.

### Result file specifiction

Result file contains multiple lines of metadata: 
* rt_initHeu = runtime to initialize heuristics
* rt_search = runtime to conduct the search
* timeout = if the planner times out
* N = the number of solutions found, which is always one.

The computed solution is then listed in three lines:
* The first line contains `Label: {label_id}`, which can be ignored in practice.
* The second line contains the a vector of format (objective_value, resource_1_value, resource_2_value, ..., resource_(M-1)_value) of the solution path.
* The third line contains the solution path (a list of vertices).

