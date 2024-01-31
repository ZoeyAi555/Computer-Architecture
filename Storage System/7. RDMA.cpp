#include <infiniband/verbs.h>
#include <iostream>

// Callback for handling completion events
void handle_completion(struct ibv_wc *wc) {
    if (wc->status != IBV_WC_SUCCESS) {
        std::cerr << "Failed status " << wc->status << " for wr_id " << wc->wr_id << std::endl;
        return;
    }

    if (wc->opcode & IBV_WC_SEND) {
        std::cout << "Send operation completed successfully." << std::endl;
    } else {
        std::cerr << "Received unexpected opcode " << wc->opcode << std::endl;
    }
}

// Poll completion queue
void poll_completion(struct ibv_cq *cq, struct ibv_qp *qp) {
    struct ibv_wc wc;
    int ne;

    do {
        ne = ibv_poll_cq(cq, 1, &wc);
        if (ne < 0) {
            std::cerr << "Poll CQ failed " << ne << std::endl;
            return;
        }
    } while (ne < 1);

    handle_completion(&wc);
}

// Main function
int main() {
    // Initialize the RDMA resources
    struct ibv_context *context = ibv_open_device(device_list[0]);
    struct ibv_pd *pd = ibv_alloc_pd(context);
    struct ibv_cq *cq = ibv_create_cq(context, 10, NULL, NULL, 0);
    struct ibv_qp *qp = ibv_create_qp(pd, &qp_init_attr);

    // Register the memory region
    struct ibv_mr *mr = ibv_reg_mr(pd, buffer, size, IBV_ACCESS_LOCAL_WRITE | IBV_ACCESS_REMOTE_WRITE);

    // Post a send operation
    struct ibv_send_wr wr, *bad_wr = NULL;
    struct ibv_sge sge;
    sge.addr = (uintptr_t)buffer;
    sge.length = size;
    sge.lkey = mr->lkey;
    wr.sg_list = &sge;
    wr.num_sge = 1;
    wr.opcode = IBV_WR_RDMA_WRITE;
    wr.send_flags = IBV_SEND_SIGNALED;
    wr.wr.rdma.remote_addr = remote_address;
    wr.wr.rdma.rkey = remote_key;

    int ret = ibv_post_send(qp, &wr, &bad_wr);
    if (ret) {
        std::cerr << "Failed to post SR " << ret << std::endl;
        return 1;
    }

    // Poll the completion queue
    poll_completion(cq, qp);

    return 0;
}
