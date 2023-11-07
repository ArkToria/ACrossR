use anyhow::anyhow;
use anyhow::Result;
use core_protobuf::acolors_proto::{
    profile_manager_server::{ProfileManager, ProfileManagerServer},
    *,
};
use lazy_static::lazy_static;
use rand::{distributions::Alphanumeric, rngs::ThreadRng, Rng};
use serial_test::serial;
use std::net::TcpListener;
use std::time::Duration;
use tokio::runtime::Runtime;
use tonic::{transport::Server, Request, Response, Status};

use crate::ffi::acolors::set_channel;
use crate::ffi::profile::*;

fn random_alphanumeric_string(rng: &mut ThreadRng, length: usize) -> String {
    rng.sample_iter(&Alphanumeric)
        .take(length)
        .map(char::from)
        .collect()
}
fn rand_node_data() -> NodeData {
    let mut rng = rand::thread_rng();
    NodeData {
        id: rng.gen(),
        name: random_alphanumeric_string(&mut rng, 10),
        group_id: rng.gen(),
        group_name: random_alphanumeric_string(&mut rng, 10),
        routing_id: rng.gen(),
        routing_name: random_alphanumeric_string(&mut rng, 10),
        protocol: random_alphanumeric_string(&mut rng, 10),
        address: random_alphanumeric_string(&mut rng, 10),
        port: rng.gen(),
        password: random_alphanumeric_string(&mut rng, 10),
        raw: random_alphanumeric_string(&mut rng, 10),
        url: random_alphanumeric_string(&mut rng, 10),
        latency: rng.gen(),
        upload: rng.gen(),
        download: rng.gen(),
        created_at: rng.gen(),
        modified_at: rng.gen(),
    }
}
fn rand_node_list() -> NodeList {
    let mut rng = rand::thread_rng();
    let length = rng.gen_range(5..15);
    let entries = (0..length).map(|_| rand_node_data()).collect();
    NodeList { length, entries }
}
fn rand_group_data() -> GroupData {
    let mut rng = rand::thread_rng();
    GroupData {
        id: rng.gen(),
        name: random_alphanumeric_string(&mut rng, 7),
        is_subscription: false,
        group_type: rng.gen_range(0..=2),
        url: random_alphanumeric_string(&mut rng, 7),
        cycle_time: rng.gen(),
        created_at: rng.gen(),
        modified_at: rng.gen(),
    }
}
fn rand_group_list() -> GroupList {
    let mut rng = rand::thread_rng();
    let length = rng.gen_range(5..15);
    let entries = (0..length).map(|_| rand_group_data()).collect();
    GroupList { length, entries }
}
lazy_static! {
    static ref GROUP_LIST: GroupList = rand_group_list();
    static ref GROUP_DATA: GroupData = rand_group_data();
    static ref NODE_LIST: NodeList = rand_node_list();
    static ref NODE_DATA: NodeData = rand_node_data();
}
const COUNT: u64 = 114514;
const GROUP_ID: i64 = 1919810;
const NODE_ID: i64 = 114;
#[derive(Default)]
pub struct TestServer {}

#[tonic::async_trait]
impl ProfileManager for TestServer {
    async fn count_groups(
        &self,
        _request: Request<CountGroupsRequest>,
    ) -> Result<Response<CountGroupsReply>, Status> {
        let reply = CountGroupsReply { count: COUNT };
        Ok(Response::new(reply))
    }

    async fn list_all_groups(
        &self,
        _request: Request<ListAllGroupsRequest>,
    ) -> Result<Response<GroupList>, Status> {
        let reply = GROUP_LIST.clone();
        Ok(Response::new(reply))
    }

    async fn count_nodes(
        &self,
        _request: Request<CountNodesRequest>,
    ) -> Result<Response<CountNodesReply>, Status> {
        let reply = CountNodesReply { count: COUNT };
        Ok(Response::new(reply))
    }

    async fn list_all_nodes(
        &self,
        _request: Request<ListAllNodesRequest>,
    ) -> Result<Response<NodeList>, Status> {
        let reply = NODE_LIST.clone();
        Ok(Response::new(reply))
    }

    async fn get_group_by_id(
        &self,
        _request: Request<GetGroupByIdRequest>,
    ) -> Result<Response<GroupData>, Status> {
        let reply = GROUP_DATA.clone();

        Ok(Response::new(reply))
    }
    async fn get_node_by_id(
        &self,
        _request: Request<GetNodeByIdRequest>,
    ) -> Result<Response<NodeData>, Status> {
        let reply = NODE_DATA.clone();

        Ok(Response::new(reply))
    }

    async fn set_group_by_id(
        &self,
        _request: Request<SetGroupByIdRequest>,
    ) -> Result<Response<SetGroupByIdReply>, Status> {
        let reply = SetGroupByIdReply {};

        Ok(Response::new(reply))
    }
    async fn set_node_by_id(
        &self,
        _request: Request<SetNodeByIdRequest>,
    ) -> Result<Response<SetNodeByIdReply>, Status> {
        let reply = SetNodeByIdReply {};

        Ok(Response::new(reply))
    }

    async fn set_node_by_url(
        &self,
        _request: Request<SetNodeByUrlRequest>,
    ) -> Result<Response<SetNodeByUrlReply>, Status> {
        let reply = SetNodeByUrlReply {};

        Ok(Response::new(reply))
    }

    async fn append_group(
        &self,
        _request: Request<AppendGroupRequest>,
    ) -> Result<Response<AppendGroupReply>, Status> {
        let reply = AppendGroupReply { group_id: GROUP_ID };

        Ok(Response::new(reply))
    }

    async fn append_node(
        &self,
        _request: Request<AppendNodeRequest>,
    ) -> Result<Response<AppendNodeReply>, Status> {
        let reply = AppendNodeReply { node_id: NODE_ID };

        Ok(Response::new(reply))
    }

    async fn append_node_by_url(
        &self,
        _request: Request<AppendNodeByUrlRequest>,
    ) -> Result<Response<AppendNodeByUrlReply>, Status> {
        let reply = AppendNodeByUrlReply { node_id: NODE_ID };

        Ok(Response::new(reply))
    }

    async fn remove_group_by_id(
        &self,
        _request: Request<RemoveGroupByIdRequest>,
    ) -> Result<Response<RemoveGroupByIdReply>, Status> {
        let reply = RemoveGroupByIdReply {};

        Ok(Response::new(reply))
    }
    async fn remove_node_by_id(
        &self,
        _request: Request<RemoveNodeByIdRequest>,
    ) -> Result<Response<RemoveNodeByIdReply>, Status> {
        let reply = RemoveNodeByIdReply {};

        Ok(Response::new(reply))
    }

    async fn update_group_by_id(
        &self,
        _request: Request<UpdateGroupByIdRequest>,
    ) -> Result<Response<UpdateGroupByIdReply>, Status> {
        let reply = UpdateGroupByIdReply {};

        Ok(Response::new(reply))
    }
    async fn empty_group_by_id(
        &self,
        _request: Request<EmptyGroupByIdRequest>,
    ) -> Result<Response<EmptyGroupByIdReply>, Status> {
        let reply = EmptyGroupByIdReply {};

        Ok(Response::new(reply))
    }
}

#[test]
#[serial]
fn test_count_groups() -> Result<()> {
    let rt = Runtime::new().unwrap();
    let port = create_server(&rt)?;
    let uri = format!("http://127.0.0.1:{port}");
    set_channel(&uri)?;
    let count = count_groups()?;
    dbg!(count);
    assert_eq!(count, COUNT);

    Ok(())
}
#[test]
#[serial]
fn test_list_all_groups() -> Result<()> {
    let rt = Runtime::new().unwrap();
    let port = create_server(&rt)?;
    let uri = format!("http://127.0.0.1:{port}");
    set_channel(&uri)?;
    let groups = list_all_groups()?;
    dbg!(&groups);
    let groups: GroupList = groups.into();
    assert_eq!(groups, GROUP_LIST.clone());

    Ok(())
}
#[test]
#[serial]
fn test_count_nodes() -> Result<()> {
    let rt = Runtime::new().unwrap();
    let port = create_server(&rt)?;
    let uri = format!("http://127.0.0.1:{port}");
    set_channel(&uri)?;
    let count = count_nodes(GROUP_ID)?;
    dbg!(count);
    assert_eq!(count, COUNT);

    Ok(())
}
#[test]
#[serial]
fn test_list_all_nodes() -> Result<()> {
    let rt = Runtime::new().unwrap();
    let port = create_server(&rt)?;
    let uri = format!("http://127.0.0.1:{port}");
    set_channel(&uri)?;
    let node_list = list_all_nodes(GROUP_ID)?;
    dbg!(&node_list);
    let node_list: NodeList = node_list.into();
    assert_eq!(node_list, NODE_LIST.clone());

    Ok(())
}
#[test]
#[serial]
fn test_get_group_by_id() -> Result<()> {
    let rt = Runtime::new().unwrap();
    let port = create_server(&rt)?;
    let uri = format!("http://127.0.0.1:{port}");
    set_channel(&uri)?;
    let group_data = get_group_by_id(GROUP_ID)?;
    dbg!(&group_data);
    let group_data: GroupData = group_data.into();
    assert_eq!(group_data, GROUP_DATA.clone());

    Ok(())
}
#[test]
#[serial]
fn test_get_node_by_id() -> Result<()> {
    let rt = Runtime::new().unwrap();
    let port = create_server(&rt)?;
    let uri = format!("http://127.0.0.1:{port}");
    set_channel(&uri)?;
    let node_data = get_node_by_id(NODE_ID)?;
    dbg!(&node_data);
    let node_data: NodeData = node_data.into();
    assert_eq!(node_data, NODE_DATA.clone());

    Ok(())
}
#[test]
#[serial]
fn test_set_group_by_id() -> Result<()> {
    let rt = Runtime::new().unwrap();
    let port = create_server(&rt)?;
    let uri = format!("http://127.0.0.1:{port}");
    set_channel(&uri)?;
    set_group_by_id(GROUP_ID, GROUP_DATA.clone().into())?;

    Ok(())
}
#[test]
#[serial]
fn test_set_node_by_id() -> Result<()> {
    let rt = Runtime::new().unwrap();
    let port = create_server(&rt)?;
    let uri = format!("http://127.0.0.1:{port}");
    set_channel(&uri)?;
    set_node_by_id(NODE_ID, NODE_DATA.clone().into())?;

    Ok(())
}
#[test]
#[serial]
fn test_set_node_by_url() -> Result<()> {
    let rt = Runtime::new().unwrap();
    let port = create_server(&rt)?;
    let uri = format!("http://127.0.0.1:{port}");
    set_channel(&uri)?;
    let mut rng = rand::thread_rng();
    set_node_by_url(GROUP_ID, random_alphanumeric_string(&mut rng, 15))?;

    Ok(())
}
#[test]
#[serial]
fn test_remove_group_by_id() -> Result<()> {
    let rt = Runtime::new().unwrap();
    let port = create_server(&rt)?;
    let uri = format!("http://127.0.0.1:{port}");
    set_channel(&uri)?;
    remove_group_by_id(GROUP_ID)?;

    Ok(())
}
#[test]
#[serial]
fn test_remove_node_by_id() -> Result<()> {
    let rt = Runtime::new().unwrap();
    let port = create_server(&rt)?;
    let uri = format!("http://127.0.0.1:{port}");
    set_channel(&uri)?;
    remove_node_by_id(NODE_ID)?;

    Ok(())
}
#[test]
#[serial]
fn test_append_group() -> Result<()> {
    let rt = Runtime::new().unwrap();
    let port = create_server(&rt)?;
    let uri = format!("http://127.0.0.1:{port}");
    set_channel(&uri)?;
    let group_id = append_group(GROUP_DATA.clone().into())?;
    dbg!(group_id);
    assert_eq!(group_id, GROUP_ID);

    Ok(())
}
#[test]
#[serial]
fn test_append_node() -> Result<()> {
    let rt = Runtime::new().unwrap();
    let port = create_server(&rt)?;
    let uri = format!("http://127.0.0.1:{port}");
    set_channel(&uri)?;
    let node_id = append_node(GROUP_ID, NODE_DATA.clone().into())?;
    dbg!(node_id);
    assert_eq!(node_id, NODE_ID);

    Ok(())
}
#[test]
#[serial]
fn test_append_node_by_url() -> Result<()> {
    let rt = Runtime::new().unwrap();
    let port = create_server(&rt)?;
    let uri = format!("http://127.0.0.1:{port}");
    set_channel(&uri)?;
    let mut rng = rand::thread_rng();
    let node_id = append_node_by_url(GROUP_ID, random_alphanumeric_string(&mut rng, 15))?;
    dbg!(node_id);
    assert_eq!(node_id, NODE_ID);

    Ok(())
}
#[test]
#[serial]
fn test_update_group_by_id() -> Result<()> {
    let rt = Runtime::new().unwrap();
    let port = create_server(&rt)?;
    let uri = format!("http://127.0.0.1:{port}");
    set_channel(&uri)?;
    update_group_by_id(GROUP_ID, true)?;

    Ok(())
}
#[test]
#[serial]
fn test_empty_group_by_id() -> Result<()> {
    let rt = Runtime::new().unwrap();
    let port = create_server(&rt)?;
    let uri = format!("http://127.0.0.1:{port}");
    set_channel(&uri)?;
    empty_group_by_id(GROUP_ID)?;

    Ok(())
}
fn create_server(rt: &Runtime) -> Result<u16, anyhow::Error> {
    let server = TestServer::default();
    let port = tcp_get_available_port(11451..19198).ok_or(anyhow!("No available ports"))?;
    let uri = format!("127.0.0.1:{port}");
    let _guard = rt.enter();
    let _server = tokio::spawn(
        Server::builder()
            .add_service(ProfileManagerServer::new(server))
            .serve(uri.parse().unwrap()),
    );
    std::thread::sleep(Duration::from_millis(100));
    Ok(port)
}

pub fn tcp_get_available_port(mut range: std::ops::Range<u16>) -> Option<u16> {
    range.find(|port| tcp_port_is_available(*port))
}

pub fn tcp_port_is_available(port: u16) -> bool {
    TcpListener::bind(("127.0.0.1", port)).is_ok()
}
