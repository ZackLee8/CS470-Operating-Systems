# set the edge case
set -e

# create the timestamp variable
time_stamp="$(date +'%Y-%m-%d_%H-%M-%S')"

# create the log file
log_file="script.log"

# create a log method to log the current time and what is happening
log(){
    echo "$(date +'%Y-%m-%d_%H-%M-%S') $1" >> "$log_file"
}

# when the script starts
log "Script is started"

# create the main directory and log it
mkdir "$time_stamp"
log "Main directory is created"

# create the sub directories
for i in {101..110}; do
    # log when the subdirectories are made
    mkdir "$time_stamp/file$i"
    log "Subdirectory file$i is Created"

    # create the 10 txt files in the sub directories and put java into each txt files
    for k in {501..510}; do 

        # create the txt files and log it
        touch "$time_stamp/file$i/tuser$k.txt"
        log "Created File: file$i/tuser$k.txt"

        # write to the txt files and log it
        echo "Java" > "$time_stamp/file$i/tuser$k.txt"
        log "java is written to tuser$k file"
    done
done

# log the ending of the scrip
log "Script is finished successfully"

