srcs-$(WITH_UART_DRV) += uart.c
srcs-$(WITH_GIC_DRV) += gic.c
srcs-$(WITH_SUNXI_UART) += sunxi_uart.c
srcs-$(CFG_SECVIDEO_PROTO) += pl111.c
srcs-$(CFG_SECVIDEO_PROTO) += tzc400.c
