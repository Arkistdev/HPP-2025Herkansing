all: circuitSatisfiability

circuitSatisfiability: circuitSatisfiability.cc
	mpic++ -O2 -o circuitSatisfiability circuitSatisfiability.cc

run: circuitSatisfiability
	mpirun -oversubscribe -np $(or $(NP),4) ./circuitSatisfiability
