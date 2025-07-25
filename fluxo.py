# Gurobi implementation of the Tool Switching Problem

from gurobipy import Model, GRB, quicksum
import math
import numpy as np

def gerar_matriz_binaria(C, l, n, seed=None):
    if seed is not None:
        np.random.seed(seed)
        
    matriz = np.zeros((n, l), dtype=int)
    
    for i in range(n):
        v = np.random.randint(0, l, size=C)  # Gera vetor com números entre 1 e l (inclusive)
        vetor_binario = np.zeros(l, dtype=int)
        vetor_binario[np.unique(v)] = 1  # Marca presença com 1 (ajustando índice para 0-based)
        matriz[i] = vetor_binario
    
    return matriz

# Exemplo de uso:
N = 15   # Número de repetições 5
M = 15  # Limite superior dos valores aleatórios 4
C = 10   # Tamanho do vetor v 3 req4

req1 = gerar_matriz_binaria(C, M, N, seed=42)
req2 = gerar_matriz_binaria(C, M, N, seed=43)
req3 = gerar_matriz_binaria(C, M, N, seed=44)
req4 = gerar_matriz_binaria(C, M, N, seed=45)
req5 = gerar_matriz_binaria(C, M, N, seed=46)

req = req5

print(req)

# Derived sets
jobs = range(1, N+1)
tools = range(1, M+1)

# Sets and parameters
# T_i: tools needed by job i (1-based indexing)
T_i = {i+1: set(np.where(req[i] == 1)[0] + 1) for i in range(N)}

# J_t: jobs that require tool t (1-based indexing)
J_t = {t+1: set(np.where(req[:, t] == 1)[0] + 1) for t in range(M)}

J_prime = {t: list(range(1, len(J_t[t]))) for t in tools}

# p: job requiring the most tools
i_counts = {i: len(T_i[i]) for i in jobs}
p = max(i_counts, key=i_counts.get)

model = Model("ToolSwitching")

# Decision variables
X = model.addVars(jobs, jobs, vtype=GRB.BINARY, name="X")
Y = model.addVars(range(0,N+3), range(0,N+3), range(1,M+1), vtype=GRB.BINARY, name="Y")

# Objective function
model.setObjective(
    quicksum(Y[i, N+1, t] for i in range(1, N) for t in tools) +
    quicksum(Y[i, N+2, t] for i in range(1, N-1) for t in tools),
    GRB.MINIMIZE
)

# === Constraints ===
# 1. Each job appears exactly once
for i in jobs:
    model.addConstr(
        quicksum(X[i, k] for k in jobs) == 1,
        name=f"JobOnce_{i}"
    )

# 2. Each position is occupied by exactly one job
for k in jobs:
    model.addConstr(
        quicksum(X[i, k] for i in jobs) == 1,
        name=f"PositionOnce_{k}"
    )

# 3. Initialization from dummy
for t in tools:
    model.addConstr(
        Y[0, 1, t] + Y[0, N+2, t] == 1,
        name=f"InitDummy_{t}"
    )

# 4. Flow conservation for internal jobs
for i in range(1, N-1):  # i in N' = 1..N-2
    for t in tools:
        model.addConstr(
            Y[i-1, i, t] + Y[N+2, i, t]
            - Y[i, N+1, t] - Y[i, i+1, t] - Y[i, N+2, t] == 0,
            name=f"Flow_{i}_{t}"
        )

# 5. Penultimate job flow (problema)
for t in tools:
   model.addConstr(
       Y[N-2, N-1, t] + Y[N+2, N-1, t]
       - Y[N-1, N, t] - Y[N-1, N+1, t] == 0,
       name=f"PenultimateFlow_{t}"
   )

# 6. Final job to dummy final
for t in tools:
    model.addConstr(
        Y[N-1, N, t] - Y[N, N+1, t] == 0,
        name=f"FinalFlow_{t}"
    )

# 7. Each tool sent to final exactly once
for t in tools:
    model.addConstr(
        quicksum(Y[i, N+1, t] for i in jobs) == 1,
        name=f"FinalOnce_{t}"
    )

# 8. Buffer (aux) flow conservation (editei para ficar N ao invés de N-1)
for t in tools:
    model.addConstr(
        quicksum(Y[i, N+2, t] for i in range(1, N))
        - quicksum(Y[N+2, i, t] for i in range(1, N)) == 0,
        name=f"BufferFlow_{t}"
    )

# 9. Tool-job linking
for i in jobs:
    for k in jobs:
        for t in T_i[i]:
            model.addConstr(
                X[i, k] <= Y[k-1, k, t],
                name=f"Link_{i}_{k}_{t}"
            )

# 10. Tool capacity per position
for k in jobs:
    model.addConstr(
        quicksum(Y[k-1, k, t] for t in tools) == C,
        name=f"Capacity_{k}"
    )

# 11. Job p in first half
half = math.ceil(N/2)
model.addConstr(
    quicksum(X[p, k] for k in range(1, half+1)) == 1,
    name="PivotJob"
)

# 12. No direct send to final for first J' jobs
for t in tools:
    for k in J_prime.get(t, []):
        model.addConstr(
            Y[k, N+1, t] == 0,
            name=f"NoDirectFinal_{k}_{t}"
        )

model.update()
# Optimize
model.optimize()

# Retrieve results
if model.status == GRB.OPTIMAL:
    solution_X = model.getAttr('x', X)
    solution_Y = model.getAttr('x', Y)
    print("Optimal Objective:", model.objVal)
    # Further output/processing here
