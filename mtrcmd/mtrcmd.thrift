struct MTRcmdReq{
	1: required string cmd = '',
	2: optional map<string, string> exdata
}

struct MTRcmdRes{
	1: required i32 status,
	2: required string error,
	3: required string info
}

service MTRcmd{
	MTRcmdRes mtrcmd(1: MTRcmdReq query);
}
