# MARCO dialects paper's benchmarks
## Run
It is possible to run the benchmarks using both Docker and Apptainer.
According to the desired container system, execute either the `docker_run.sh` or `apptainer_run.sh` scripts.
The `docker.slurm` and `apptainer.slurm` are also provided to run the configurations on a computing cluster managed by SLURM.

# Results
At the end of the benchmarks execution, an archive containing the logs and results is produced under the `output` folder.
Once unpacked, it is possible to use the `csv_exporter` tool to automatically extract the measurements from the raw log files and export them in CSV format. The CSV output can be inserted into the first page of the `measurements.ods` spreadsheet template. The remaining pages are views on the first one, each of which focusing on a specific benchmark dimension.
