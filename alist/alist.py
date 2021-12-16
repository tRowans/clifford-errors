import numpy as np

def save_alist(name, mat, j=None, k=None):

    
    H=np.copy(mat)
    H=H.T
    
    '''
    Function converts parity check matrix into the format required for the RN decoder
    '''

    if j is None:
        # column weight
        # j = H[:, 0].sum()
        j=int(max(H.sum(axis=0)))


    if k is None:
        # row weight
        # k = H[0, :].sum()
        k=int(max(H.sum(axis=1)))


    m, n = H.shape # rows, cols
    f = open(name, 'w')
    print(n, m, file=f)
    print(j, k, file=f)

    for col in range(n):
        print( int(H[:, col].sum()), end=" ", file=f)
    print(file=f)
    for row in range(m):
        print( int(H[row, :].sum()), end=" ", file=f)
    print(file=f)

    for col in range(n):
        for row in range(m):
            if H[row, col]:
                print( row+1, end=" ", file=f)
        print(file=f)

    for row in range(m):
        for col in range(n):
            if H[row, col]:
                print(col+1, end=" ", file=f)
        print(file=f)
    f.close()

def save_logical_sparse(filename,logical_ops):

    n=len(logical_ops[0])
    k=len(logical_ops)
    
    row_weights=np.sum(logical_ops,axis=1)
    d=min(row_weights)
    
    sparse_logical_ops=np.array(list(map(lambda x: np.nonzero(x)[0],logical_ops)))
    
    # print(sparse_logical_ops)
    
    with open(filename, "w") as f:
        f.write(f"{n} {k}\n")
        f.write(f"{d}\n")
        for i,rw in enumerate(row_weights):
            f.write(f"{rw} ")
        f.write("\n")
        f.close()
    with open(filename, "ab") as f:
        for i, logical in enumerate(sparse_logical_ops):
            np.savetxt(f,logical.reshape(( 1, row_weights[i] )),fmt="%i",delimiter=" ")

def save_hz_from_txt(L):
    file = 'cubic_L=' + str(L)
    hz = np.loadtxt(file + '.txt')
    hz_zip = hz[:,~np.all(hz == 0, axis=0)] # Remove 0 columns
    save_alist(file + '.alist', hz_zip)
    file = 'rhombic1_L=' + str(L)
    hz = np.loadtxt(file + '.txt')
    hz_zip = hz[:,~np.all(hz == 0, axis=0)] # Remove 0 columns
    save_alist(file + '.alist', hz_zip)
    file = 'rhombic2_L=' + str(L)
    hz = np.loadtxt(file + '.txt')
    hz_zip = hz[:,~np.all(hz == 0, axis=0)] # Remove 0 columns
    save_alist(file + '.alist', hz_zip)

if __name__ == "__main__":
    L_min = 6
    L_max = 12
    for L in range(L_min, L_max + 1, 2):
        save_hz_from_txt(L)

