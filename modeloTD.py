from gurobipy import Model, GRB, quicksum
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
C = 5   # Tamanho do vetor v
l = 15  # Limite superior dos valores aleatórios
n = 11   # Número de repetições

req1 = gerar_matriz_binaria(C, l, n, seed=42)
print(req1)
req2 = gerar_matriz_binaria(C, l, n, seed=43)
print(req2)
req3 = gerar_matriz_binaria(C, l, n, seed=44)
print(req3)
req4 = gerar_matriz_binaria(C, l, n, seed=45)
print(req4)
req5 = gerar_matriz_binaria(C, l, n, seed=46)
print(req5)

req = req4

J = K = list(range(0,n))      # {1, ..., n}
T = list(range(0,l))          # {1, ..., l}
K_no1 = K[1:]                 # K \ {1} -> ou seja, de 2 até n

# Modelo
model = Model("modelo_binario")

# Variáveis
w = model.addVars(J, K, vtype=GRB.BINARY, name="w")
y = model.addVars(K, T, vtype=GRB.BINARY, name="y")
z = model.addVars(K_no1, T, vtype=GRB.BINARY, name="z")

# Função objetivo
model.setObjective(quicksum(z[k, t] for k in K_no1 for t in T), GRB.MINIMIZE)

# Restrição 1: cada k é atribuído a um único j
for k in K:
    model.addConstr(quicksum(w[j, k] for j in J) == 1, name=f"R1_k{k}")

# Restrição 2: cada j recebe um único k
for j in J:
    model.addConstr(quicksum(w[j, k] for k in K) == 1, name=f"R2_j{j}")

# Restrição 3: soma dos w_{jk} ≤ y_{kt}
for k in K:
    for t in T:
        model.addConstr(
            quicksum(w[j, k] for j in J if req[j][t] == 1) <= y[k, t],
            name=f"R3_k{k}_t{t}"
        )

# Restrição 4: capacidade
for k in K:
    model.addConstr(quicksum(y[k, t] for t in T) <= C, name=f"R4_k{k}")

# Restrição 5: y_{kt} - y_{(k-1)t} ≤ z_{kt}
for k in K_no1:
    for t in T:
        model.addConstr(y[k, t] - y[k - 1, t] <= z[k, t], name=f"R5_k{k}_t{t}")

# Resolver
model.optimize()

# Mostrar solução
#for v in model.getVars():
#   print(f"{v.VarName} = {v.X}")