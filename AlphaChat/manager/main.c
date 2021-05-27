/*

서버

1. mcast에 서버의 정보를 보냄 (IP, PORT)
2. udp를 이용하여 클라이언트가 보내온 하트비트를 수신하여 클라이언트의 정보를 저장한다.
3. 클라이언트 별로 tcp client socket과 tcp server socket을 만든다
4. 클라이언트에서 tcp client socket 으로 자신이 원하는 리퀘스트를 전송한다.
5. 서버는 그 리퀘스를 전송받아 처리를 한다. 
6. 서버는 치리 결과를 클라이언트에 접속되어있는 tcp client socket으로 보낸다.


*/

int main()
{
    
    return 0;
}