# MARCO dialects paper's benchmarks
## Models
The models under test can be found in the `data/models` folder.
The parameters controlling the number of equations are automatically changed by the infrastructure.

## Run
It is possible to run the benchmarks using both Docker and Apptainer.
According to the desired container system, execute either the `docker_run.sh` or `apptainer_run.sh` scripts.
The `docker.slurm` and `apptainer.slurm` are also provided to run the configurations on a computing cluster managed by SLURM.

## Results
At the end of the benchmarks execution, an archive containing the logs and results is produced under the `output` folder. For each model, two `.csv` files are also generated, one for the Explicit Euler and one for the IDA benchmark. Each CSV output can be inserted into the first page of the `measurements.ods` spreadsheet template, available in each of the model folders. The remaining pages are views on the first one, each of which focusing on a specific benchmark dimension.
