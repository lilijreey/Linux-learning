%%%-------------------------------------------------------------------
%%% @author zl
%%% @doc  zk 压测
%%% erl -kernel error_logger silent -sasl sasl_error_logger false
%%%        
%%% @end
%%%-------------------------------------------------------------------

-module(worker).

-export([start_link/1,
start_link/0,
         start/1]).


times(0, Fn) -> 
    Fn(0);
times(N, Fn) -> 
    Fn(N),
    times(N-1, Fn).
sleep(Msec) ->
    receive _ -> ok
    after Msec-> ok
    end.


task() -> 
    {ok, P} = erlzk:connect([{"127.0.0.1", 2181}], 3000),
    SelfStr = erlang:pid_to_list(self()),
    Root = "/test/" ++ SelfStr,
    case erlzk:create(P, Root) of 
        {ok, _} -> 
            times(rand:uniform(200), 
                  fun(N) -> 
                          sleep(rand:uniform(200)),
                          erlzk:create(P, Root ++ "/jfoefejfoefjefo" ++ integer_to_list(N)),
                          erlzk:get_children(P,Root)
                  end);
        {error, node_exists}->
            times(rand:uniform(200), 
                  fun(N) -> 
                          sleep(rand:uniform(200)),
                          erlzk:delete(P, Root ++ "/jfoefejfoefjefo" ++ integer_to_list(N)),
                          erlzk:get_children(P,Root)
                  end);
        Err ->
            io:format("create node ~p error ~p~n", [Root, Err])
    end,
    times(rand:uniform(25), 
          fun(N) -> 
                  erlzk:delete(P, Root ++ "/jfoefejfoefjefo" ++ integer_to_list(N))
          end),
    erlzk:close(P).


start(N) ->
    erlzk:start(),

    times(N, fun(_N) -> spawn(fun task/0) end),
    io:format("start over~n"),
    ok.

