import pyAesCrypt
# encryption/decryption buffer size - 64K
bufferSize = 64 * 1024
password = "foopassword"
# encrypt
pyAesCrypt.encryptFile("graph.pb", "graph.pb.aes", password, bufferSize)
# decrypt
pyAesCrypt.decryptFile("graph.pb.aes", "graph-decrypt.pb", password, bufferSize)