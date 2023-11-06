use std::net::TcpListener;

pub fn tcp_get_available_ports(range: std::ops::Range<u16>, count: usize) -> Option<Vec<u16>> {
    let mut ports = Vec::with_capacity(count);
    for i in range {
        if tcp_port_is_available(i) {
            ports.push(i);
            if ports.len() >= count {
                break;
            }
        }
    }
    if ports.len() < count {
        None
    } else {
        Some(ports)
    }
}

pub fn tcp_get_available_port(mut range: std::ops::Range<u16>) -> Option<u16> {
    range.find(|port| tcp_port_is_available(*port))
}

pub fn tcp_port_is_available(port: u16) -> bool {
    TcpListener::bind(("127.0.0.1", port)).is_ok()
}
