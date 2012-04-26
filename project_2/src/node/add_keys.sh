for n in $(seq 175 198); do
    ssh tux$n echo key added for tux$n;
done
