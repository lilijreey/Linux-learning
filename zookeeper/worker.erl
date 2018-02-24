%%%-------------------------------------------------------------------
%%% @author zl
%%% @doc  zk 压测
%%% erl -kernel error_logger silent -sasl sasl_error_logger false
%%%        
%%% @end
%%%-------------------------------------------------------------------

-module(worker).

-export([
         create/0,
         create/1,
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

create() -> 
    create(1000).

create(N) -> 
    {ok, P} = erlzk:connect([{"127.0.0.1", 2181}], 3000),
    Ref = erlang:make_ref(),
    Root = "/test/" ++ erlang:ref_to_list(Ref),
    {ok, _}  = erlzk:create(P, Root),
    Base = rand:uniform(N), 
    times(N,
          fun(I) -> 
                  %sleep(rand:uniform(200)),
                  case erlzk:create(P, Root ++ "/" ++ integer_to_list(Base+I)) of
                      {error, E} -> 
                          io:format("Error ~p ~p~n", [Root, E]);
                      %%{ok,_} when I rem 100 =:= 0 -> 
                      %%    io:format(" ~p create node count ~p~n", [Root, I]);
                      _ -> ok
                      %%{ok, _Path} ->
                      %%    io:format(" ~p create node count ~p~n", [Root, _Path])
                  end
          end),
    io:format("create over~n"),
    {ok, C} = erlzk:get_children(P, Root),
    io:format("create over count ~p ~n", [length(C)]),
    sleep(1000),
    erlzk:close(P),
    ok.



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

    times(N, fun(_N) -> spawn(fun create/0) end),
    io:format("start over~n"),
    ok.

