# MARCO dialects paper's benchmarks
## Model
The model under test can be found in `data/src/ThermalChipOO.mo`.
The various tested configurations, each differing in the `N`, `M`, and `P` parameters, can be found in the `data/src/configs` folder.

## Run
It is possible to run the benchmarks using both Docker and Apptainer.
According to the desired container system, execute either the `docker_run.sh` or `apptainer_run.sh` scripts.
The `docker.slurm` and `apptainer.slurm` are also provided to run the configurations on a computing cluster managed by SLURM.

## Results
At the end of the benchmarks execution, an archive containing the logs and results is produced under the `output` folder. Two `.csv` files are also generated, one for the Forward Euler and one for the IDA benchmark. Each CSV output can be inserted into the first page of the `measurements.ods` spreadsheet template. The remaining pages are views on the first one, each of which focusing on a specific benchmark dimension.
