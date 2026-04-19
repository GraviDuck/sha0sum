#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef struct {
    uint32_t state[5];
    uint32_t count[2];
    uint8_t  buffer[64];
} SHA0_CTX;

#define ROL(x, n) ((x << n) | (x >> (32 - n)))

void sha0_transform(uint32_t state[5], const uint8_t buffer[64]) {
    uint32_t a, b, c, d, e, w[80];
    int i;

    for (i = 0; i < 16; i++) {
        w[i] = (buffer[i*4] << 24) | (buffer[i*4+1] << 16) | (buffer[i*4+2] << 8) | (buffer[i*4+3]);
    }
    for (i = 16; i < 80; i++) {
        w[i] = w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16];
    }

    a = state[0]; b = state[1]; c = state[2]; d = state[3]; e = state[4];

    for (i = 0; i < 80; i++) {
        uint32_t f, k;
        if (i < 20) { f = (b & c) | (~b & d); k = 0x5A827999; }
        else if (i < 40) { f = b ^ c ^ d; k = 0x6ED9EBA1; }
        else if (i < 60) { f = (b & c) | (b & d) | (c & d); k = 0x8F1BBCDC; }
        else { f = b ^ c ^ d; k = 0xCA62C1D6; }

        uint32_t temp = ROL(a, 5) + f + e + k + w[i];
        e = d; d = c; c = ROL(b, 30); b = a; a = temp;
    }
    state[0] += a; state[1] += b; state[2] += c; state[3] += d; state[4] += e;
}

void sha0_init(SHA0_CTX *ctx) {
    ctx->state[0] = 0x67452301; ctx->state[1] = 0xEFCDAB89; ctx->state[2] = 0x98BADCFE;
    ctx->state[3] = 0x10325476; ctx->state[4] = 0xC3D2E1F0;
    ctx->count[0] = ctx->count[1] = 0;
}

void sha0_update(SHA0_CTX *ctx, const uint8_t *data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        ctx->buffer[ctx->count[0] & 63] = data[i];
        if (++ctx->count[0] == 0) ctx->count[1]++;
        if ((ctx->count[0] & 63) == 0) sha0_transform(ctx->state, ctx->buffer);
    }
}

void sha0_final(SHA0_CTX *ctx, uint8_t hash[20]) {
    uint32_t low = ctx->count[0] << 3;
    uint32_t high = (ctx->count[1] << 3) | (ctx->count[0] >> 29);
    uint8_t pad = 0x80;
    sha0_update(ctx, &pad, 1);
    while ((ctx->count[0] & 63) != 56) { pad = 0x00; sha0_update(ctx, &pad, 1); }
    for (int i = 0; i < 8; i++) {
        pad = (i < 4 ? high : low) >> (24 - (i % 4) * 8);
        sha0_update(ctx, &pad, 1);
    }
    for (int i = 0; i < 20; i++) hash[i] = (ctx->state[i/4] >> (24 - (i%4)*8)) & 255;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Error: No input provided. Please specify a file path or a text string.\n");
        return 1;
    }

    SHA0_CTX ctx;
    uint8_t hash[20]; // Arreglado: tamaño del hash
    sha0_init(&ctx);

    // Usamos argv[1] para referirnos al primer argumento pasado
    FILE *f = fopen(argv[1], "rb"); 
    
    if (f) {
        // BUFFER DE 32KB PARA VELOCIDAD
        static uint8_t buf[32768]; 
        size_t n;
        while ((n = fread(buf, 1, sizeof(buf), f)) > 0) {
            sha0_update(&ctx, buf, n);
        }
        fclose(f);
        sha0_final(&ctx, hash);
        
        for (int i = 0; i < 20; i++) printf("%02x", hash[i]);
        printf(" *%s\n", argv[1]);
    } 
    else {
        // MODO TEXTO
        sha0_update(&ctx, (const uint8_t *)argv[1], strlen(argv[1]));
        sha0_final(&ctx, hash);
        
        for (int i = 0; i < 20; i++) printf("%02x", hash[i]);
        printf("  \"%s\"\n", argv[1]);
    }
    return 0;
}
